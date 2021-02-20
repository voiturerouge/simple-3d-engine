#include "renderer.h"
#include "math.h"

Renderer::Renderer(int width, int height)
{
    p_image = new QImage(width, height, QImage::Format_ARGB32);
    p_zBuffer = new float[width * height]; // z depth of pixel (x, y) will be stored at index 'y*width + x'
}

Renderer::~Renderer()
{
    delete p_image;
    delete []p_zBuffer;
}


void Renderer::setPixel(const int& x, const int& y, const float& z_inv, const QColor& color)
{
    if (p_zBuffer[y * p_image->width() + x] > (1/z_inv))
    {
        QRgb* line = (QRgb*) p_image->scanLine(y);
        line[x] = qRgba(color.red(), color.green(), color.blue(), color.alpha());
        p_zBuffer[y * p_image->width() + x] = 1/z_inv;
    }
    else {
//        qDebug("error");
    }
}

void Renderer::fillScanLine(const int& x1, const float& z1_inv, const int& x2, const float& z2_inv, int& y, const QColor& color)
{
    if (x1 == x2) {
        setPixel(x1, y, z1_inv, color);
        return;
    }

    // fill line
    int x = (x1 < x2) ? x1 : x2;
    int xe = (x1 < x2) ? x2 : x1;
    for (; x <= xe; x++)
    {
        float z_inv = z1_inv + (x - x1) * (z2_inv - z1_inv) / (x2 - x1);
        setPixel(x, y, z_inv, color);
    }
}

void Renderer::getBLineNextSegment(const QVector2D& seg1start, QVector2D& seg1end, QVector2D& seg2start, const QVector2D& lineend, const int& dx, const int& dy, const int& sx, const int& sy, int& err)
{
    bool exit = false;
    seg1end.setX(seg1start.x());
    seg1end.setY(seg1start.y());
    seg2start.setX(seg1start.x());
    seg2start.setY(seg1start.y());

    while (true) {
        if (seg1start == lineend)
            break;
        int e2 = 2 * err;
        if (e2 < dx) {
            err += dx;
            seg2start.setY(seg2start.y() + sy);
            exit = true;
        }
        if (e2 > -dy) {
            err -= dy;
            if (exit) {
                seg1end.setX(seg2start.x());
            }
            seg2start.setX(seg2start.x() + sx);
        }
        if (exit)
            break;
    }
}

void Renderer::getBLineNextPoint(int& xnext, int& ynext, int& xprev, int& yprev, const QVector3D& vend, const int& dx, const int& dy, const int& sx, const int& sy, int& err)
{
    bool exit = false;
    int xend = (int) round(vend.x());
    int yend = (int) round(vend.y());
    yprev = ynext;
    xprev = xnext;


    while (true) {
        if ((xnext == xend) && (ynext == yend))
            break;
        int e2 = 2 * err;
        if (e2 < dx) {
            err += dx;
            yprev = ynext;
            ynext += sy;
            exit = true;
        }
        if (e2 > -dy) {
            err -= dy;
            if (exit) {
                xprev = xnext;
            }
            xnext += sx;
        }
        if (exit)
            break;
    }
}

void Renderer::initBLine(const QVector3D& v1, const QVector3D& v2, int& dx, int& dy, int& sx, int& sy, int& err)
{
    int x1 = (int) round(v1.x());
    int y1 = (int) round(v1.y());
    int x2 = (int) round(v2.x());
    int y2 = (int) round(v2.y());

    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    sx = (x1 < x2) ? 1 : -1; // x increment
    sy = (y1 < y2) ? 1 : -1; // y increment
    err = dx - dy;
}

void Renderer::drawBLine(const QVector3D& point1, const QVector3D& point2, const QColor& color)
{
    int   x1 = (int) round(point1.x());
    int   y1 = (int) round(point1.y());
    float z1 = point1.z();
    int   x2 = (int) round(point2.x());
    int   y2 = (int) round(point2.y());
    float z2 = point2.z();

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1; // x increment
    int sy = (y1 < y2) ? 1 : -1; // y increment
    int err = dx - dy;
    int x = x1;
    int y = y1;
    float z = z1;

    while (true) {
        setPixel(x, y, -100, color);

        if ((x == x2) && (y == y2))
            break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
        z = (x - x1) * (z2 - z1) / (x2 - x1) + z1;
    }
}

void Renderer::fillBTriangle(std::vector<QVector3D>& triangleP, const QColor& color)
{
    // sort vertices
    std::sort(triangleP.begin(),
              triangleP.end(),
              [](QVector3D va, QVector3D vb) { return va.y() < vb.y(); });

    int dx1, dy1, sx1, sy1, err1;
    int dx2, dy2, sx2, sy2, err2;

    int xnext1, ynext1, xprev1, yprev1;
    int xnext2, ynext2, xprev2, yprev2;

    int   xa = (int) round(triangleP[0].x());
    int   ya = (int) round(triangleP[0].y());
    float zaInv = 1/triangleP[0].z();
    int   xb = (int) round(triangleP[1].x());
    int   yb = (int) round(triangleP[1].y());
    float zbInv = 1/triangleP[1].z();
    int   xc = (int) round(triangleP[2].x());
    int   yc = (int) round(triangleP[2].y());
    float zcInv = 1/triangleP[2].z();

    // top triangle
    // line (A,B)
    initBLine(triangleP[0], triangleP[1], dx1, dy1, sx1, sy1, err1);
    xnext1 = (int) round(triangleP[0].x()),
    ynext1 = (int) round(triangleP[0].y());

    // line (A,C)
    initBLine(triangleP[0], triangleP[2], dx2, dy2, sx2, sy2, err2);
    xnext2 = (int) round(triangleP[0].x()),
    ynext2 = (int) round(triangleP[0].y());

    for (int y = triangleP[0].y(); y <= triangleP[1].y(); y++)
    {
        getBLineNextPoint(xnext1, ynext1, xprev1, yprev1, triangleP[1], dx1, dy1, sx1, sy1, err1); // line (A,B)
        float z1Inv = zaInv + (zbInv - zaInv) * (y - ya) / (yb - ya);
        getBLineNextPoint(xnext2, ynext2, xprev2, yprev2, triangleP[2], dx2, dy2, sx2, sy2, err2); // line (A,C)
        float z2Inv = zaInv + (zcInv - zaInv) * (y - ya) / (yc - ya);
        fillScanLine(xprev1, z1Inv, xprev2, z2Inv, y, color);
    }

    // bottom triangle
    // line (C,B)
    initBLine(triangleP[2], triangleP[1], dx1, dy1, sx1, sy1, err1);
    xnext1 = (int) round(triangleP[2].x()),
    ynext1 = (int) round(triangleP[2].y());

    // line (C,A)
    initBLine(triangleP[2], triangleP[0], dx2, dy2, sx2, sy2, err2);
    xnext2 = (int) round(triangleP[2].x()),
    ynext2 = (int) round(triangleP[2].y());

    for (int y = triangleP[2].y(); y >= triangleP[1].y(); y--)
    {
        getBLineNextPoint(xnext1, ynext1, xprev1, yprev1, triangleP[1], dx1, dy1, sx1, sy1, err1); // line (C,B)
        float z1Inv = zcInv + (zbInv - zcInv) * (y - yc) / (yb - yc);
        getBLineNextPoint(xnext2, ynext2, xprev2, yprev2, triangleP[0], dx2, dy2, sx2, sy2, err2); // line (C,A)
        float z2Inv = zcInv + (zaInv - zcInv) * (y - yc) / (ya - yc);
        fillScanLine(xprev1, z1Inv, xprev2, z2Inv, y, color);
    }
}


void Renderer::clear(const QColor& color)
{
    p_image->fill(color);
    std::fill(p_zBuffer, p_zBuffer+(p_image->width()*p_image->height()), std::numeric_limits<float>::max());
}

// TODO pass the camera and the meshes to the constructor of the renderer?
void Renderer::render(const Camera& camera, const QVector<Mesh>& meshList)
{
    // View matrix
    QMatrix4x4 view;
    view.setToIdentity();
    view.lookAt(camera.getPosition(), camera.getTarget(), camera.getDirection());

    // Projection matrix
    QMatrix4x4 projection;
    projection.setToIdentity();
    projection.perspective(45, (float)p_image->width()/(float)p_image->height(), 1.0, 1000.0);
    QRect viewport(0, 0, p_image->width(), p_image->height());

    for(const Mesh& mesh : meshList) {

        // World matrix
        QMatrix4x4 world;
        world.setToIdentity();
        world.scale(mesh.getScale());
        world.translate(mesh.getPosition());
        world.rotate(mesh.getRotation().x(), 1, 0, 0);
        world.rotate(mesh.getRotation().y(), 0, 1, 0);
        world.rotate(mesh.getRotation().z(), 0, 0, 1);

        // Draw faces
        for (int i=0; i<mesh.m_faces.size(); i++)
        {
//            std::vector<QVector3D> triangleC // triangle in the camera space
//            {
//                view * world * mesh.m_vertices[mesh.m_faces[i][0]],
//                view * world * mesh.m_vertices[mesh.m_faces[i][1]],
//                view * world * mesh.m_vertices[mesh.m_faces[i][2]]
//            };
            std::vector<QVector3D> triangleP // triangle projected in the screen space
            {
                mesh.m_vertices[mesh.m_faces[i][0]].project(view*world, projection, viewport),
                mesh.m_vertices[mesh.m_faces[i][1]].project(view*world, projection, viewport),
                mesh.m_vertices[mesh.m_faces[i][2]].project(view*world, projection, viewport)
            };


            // triangle
            if (i%2) {
                fillBTriangle(triangleP, Qt::blue);
            }
            else {
                fillBTriangle(triangleP, Qt::cyan);
            }

            // wireframe
//            drawBLine(v1, v2, mesh.getColor());
//            drawBLine(v2, v3, mesh.getColor());
//            drawBLine(v3, v1, mesh.getColor());
        }
    }
}

QImage Renderer::getImage() const
{
    return *p_image;
}
