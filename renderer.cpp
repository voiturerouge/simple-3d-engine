#include "renderer.h"

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


void Renderer::setPixel(const int& x, const int& y, const float& z, const QColor& color)
{
    if (p_zBuffer[y * p_image->width() + x] > z)
    {
        QRgb* line = (QRgb*) p_image->scanLine(y);
        line[x] = qRgba(color.red(), color.green(), color.blue(), color.alpha());
        p_zBuffer[y * p_image->width() + x] = z;
    }
    else {
//        qDebug("error");
    }
}

void Renderer::fillScanLine(const QVector3D& pa, const QVector3D& pb, const QVector3D& pc, const int& x1, const int& x2, const int& y, const QColor& color)
{
    int   xa = (int) pa.x();
    float za = pa.z();
    int   xb = (int) pb.x();
    float zb = pb.z();
    int   xc = (int) pc.x();
    float zc = pc.z();

    if (x1 == x2) {
        float z = (xa != xb) ? (x1 - xa) * (zb - za) / (xb - xa) + za : zb;
        setPixel(x1, y, z, color);
        return;
    }

    int xs, xe;
    float zs, ze;

    if (x1 < x2) {
        // start point is on line (A,B)
        xs = x1;
        zs = (xa != xb) ? (xs - xa) * (zb - za) / (xb - xa) + za : zb;
        // end point is on line (A, C)
        xe = x2;
        ze = (xa != xc) ? (xe - xa) * (zc - za) / (xc - xa) + za : zc;
    }

    else {
        // start point is on line (A,C)
        xs = x2;
        zs = (xa != xc) ? (xs - xa) * (zc - za) / (xc - xa) + za : zc;
        // end point is on line (A, B)
        xe = x1;
        ze = (xa != xb) ? (xe - xa) * (zb - za) / (xb - xa) + za : zb;
    }

    // fill line
    for (int x = xs; x <= xe; x++)
    {
        float z = (xe != xs) ? (x - xs) * (ze - zs) / (xe - xs) + zs : zs;
        setPixel(x, y, z, color);
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
    int xend = (int) vend.x();
    int yend = (int) vend.y();
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
    int x1 = (int) v1.x();
    int y1 = (int) v1.y();
    int x2 = (int) v2.x();
    int y2 = (int) v2.y();

    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    sx = (x1 < x2) ? 1 : -1; // x increment
    sy = (y1 < y2) ? 1 : -1; // y increment
    err = dx - dy;
}

void Renderer::drawBLine(const QVector3D& point1, const QVector3D& point2, const QColor& color)
{
    int   x1 = (int) point1.x();
    int   y1 = (int) point1.y();
    float z1 = point1.z();
    int   x2 = (int) point2.x();
    int   y2 = (int) point2.y();
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

void Renderer::fillBTriangle(const QVector3D& v1, const QVector3D& v2, const QVector3D& v3, const QColor& color)
{
    // sort vertices
    std::vector<QVector3D> v{v1, v2, v3};
    std::sort(v.begin(),
              v.end(),
              [](QVector3D va, QVector3D vb) { return va.y() < vb.y(); });

    int dx1, dy1, sx1, sy1, err1;
    int dx2, dy2, sx2, sy2, err2;

    int xnext1, ynext1, xprev1, yprev1;
    int xnext2, ynext2, xprev2, yprev2;

    QColor blue = Qt::blue;
    QColor magenta = Qt::magenta;

    // top triangle
    // line (A,B)
    initBLine(v[0], v[1], dx1, dy1, sx1, sy1, err1);
    xnext1 = (int) v[0].x(),
    ynext1 = (int) v[0].y();

    // line (A,C)
    initBLine(v[0], v[2], dx2, dy2, sx2, sy2, err2);
    xnext2 = (int) v[0].x(),
    ynext2 = (int) v[0].y();

    for (int y = v[0].y(); y <= v[1].y(); y++)
    {
        getBLineNextPoint(xnext1, ynext1, xprev1, yprev1, v[1], dx1, dy1, sx1, sy1, err1);
        getBLineNextPoint(xnext2, ynext2, xprev2, yprev2, v[2], dx2, dy2, sx2, sy2, err2);
        fillScanLine(v[0], v[1], v[2], xprev1, xprev2, y, color);
    }

    // bottom triangle
    // line (C, B)
    initBLine(v[2], v[1], dx1, dy1, sx1, sy1, err1);
    xnext1 = (int) v[2].x(),
    ynext1 = (int) v[2].y();

    // line (C, A)
    initBLine(v[2], v[0], dx2, dy2, sx2, sy2, err2);
    xnext2 = (int) v[2].x(),
    ynext2 = (int) v[2].y();

    for (int y = v[2].y(); y >= v[1].y(); y--)
    {
        getBLineNextPoint(xnext1, ynext1, xprev1, yprev1, v[1], dx1, dy1, sx1, sy1, err1);
        getBLineNextPoint(xnext2, ynext2, xprev2, yprev2, v[0], dx2, dy2, sx2, sy2, err2);
        fillScanLine(v[2], v[1], v[0], xprev1, xprev2, y, color);
    }
}


void Renderer::clear(const QColor& color)
{
    p_image->fill(color);
    std::fill(p_zBuffer, p_zBuffer+(p_image->width()*p_image->height()), std::numeric_limits<float>::max());
}

// TODO pass the camera and the meshes to the constructor?
void Renderer::render(const Camera& camera, const QVector<Mesh>& meshList)
{
    // View
    QMatrix4x4 view;
    view.setToIdentity();
    view.lookAt(camera.getPosition(), camera.getTarget(), camera.getDirection());

    // Projection
    QMatrix4x4 projection;
    projection.setToIdentity();
    projection.perspective(45, (float)p_image->width()/(float)p_image->height(), 1.0, 1000.0);
    QRect viewport(0, 0, p_image->width(), p_image->height());

    for(const Mesh& mesh : meshList) {

        // World
        QMatrix4x4 world;
        world.setToIdentity();
        world.scale(mesh.getScale());
        world.translate(mesh.getPosition());
        world.rotate(mesh.getRotation().x(), 1, 0, 0);
        world.rotate(mesh.getRotation().y(), 0, 1, 0);
        world.rotate(mesh.getRotation().z(), 0, 0, 1);

        // Draw
        for (int i=0; i<mesh.m_faces.size(); i++)
        {
            QVector<int> face = mesh.m_faces[i];

            QVector3D v1 = mesh.m_vertices[face[0]].project(view*world, projection, viewport);
            QVector3D v2 = mesh.m_vertices[face[1]].project(view*world, projection, viewport);
            QVector3D v3 = mesh.m_vertices[face[2]].project(view*world, projection, viewport);

            // triangle
            if (i%2) {
                fillBTriangle(v1, v2, v3, Qt::blue);
            }
            else {
                fillBTriangle(v1, v2, v3, Qt::cyan);
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
