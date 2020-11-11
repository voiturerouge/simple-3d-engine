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

QImage Renderer::getImage() const
{
    return *p_image;
}

void Renderer::clear(const QColor& color)
{
    p_image->fill(color);
    std::fill(p_zBuffer, p_zBuffer+(p_image->width()*p_image->height()), std::numeric_limits<float>::max());
}

void Renderer::setPixel(const int& x, const int& y, const float& z, const QColor& color)
{
    if (p_zBuffer[y * p_image->width() + x] > z)
    {
        QRgb* line = (QRgb*) p_image->scanLine(y);
        line[x] = qRgba(color.red(), color.green(), color.blue(), color.alpha());
        p_zBuffer[y * p_image->width() + x] = z;
    }
}


void Renderer::fillBTriangle(const QVector3D& point1, const QVector3D& point2, const QVector3D& point3, const QColor& color)
{
    // sort vertices
    std::vector<std::vector<int>> points{{(int) point1.x(), (int) point1.y()},
                                         {(int) point2.x(), (int) point2.y()},
                                         {(int) point3.x(), (int) point3.y()}};
    std::sort(points.begin(),
              points.end(),
              [](std::vector<int> pointa, std::vector<int> pointb) { return pointa[1] < pointb[1]; });

    int   x1 = points[0][0];
    int   y1 = points[0][1];
    float z1 = points[0][2];
    int   x2 = points[1][0];
    int   y2 = points[1][1];
    float z2 = points[1][2];
    int   x3 = points[2][0];
    int   y3 = points[2][1];
    float z3 = points[2][2];

    int dx1, dy1, sx1, sy1, err1;
    int dx2, dy2, sx2, sy2, err2;

    int xnext1, ynext1, xprev1, yprev1;
    int xnext2, ynext2, xprev2, yprev2;

    QColor blue = Qt::blue;
    QColor magenta = Qt::magenta;

    // top triangle
    xnext1 = x1, ynext1 = y1;
    xnext2 = x1, ynext2 = y1;
    initBLine(x1, y1, x2, y2, dx1, dy1, sx1, sy1, err1); // line point1 to point2
    initBLine(x1, y1, x3, y3, dx2, dy2, sx2, sy2, err2); // line point1 to point2
    for (int y = y1; y <= y2; y++)
    {
        // get
        getBLineNextPoint(xnext1, ynext1, xprev1, yprev1, x2, y2, dx1, dy1, sx1, sy1, err1);
        getBLineNextPoint(xnext2, ynext2, xprev2, yprev2, x3, y3, dx2, dy2, sx2, sy2, err2);

        int xmax = (xprev1 < xprev2) ? xprev2 : xprev1;
//        float zprev1 = (xprev1 - x1) * (z2 - z1) / (x2 - x1) + z1;
//        float zprev2 = (xprev2 - x1) * (z3 - z1) / (x3 - x1) + z3;

        // fill line
        for (int x = (xprev1 < xprev2) ? xprev1:xprev2; x <= xmax; x++)
        {
            //float z = (x - x1) * (z2 - z1) / (x2 - x1) + z1;

            setPixel(x, y, 100, blue);
        }
    }

    // bottom triangle
    xnext1 = x3, ynext1 = y3;
    xnext2 = x3, ynext2 = y3;
    initBLine(x3, y3, x2, y2, dx1, dy1, sx1, sy1, err1); // line point3 to point2
    initBLine(x3, y3, x1, y1, dx2, dy2, sx2, sy2, err2); // line point3 to point1
    for (int y = y3; y >= y2; y--)
    {
        getBLineNextPoint(xnext1, ynext1, xprev1, yprev1, x2, y2, dx1, dy1, sx1, sy1, err1);
        getBLineNextPoint(xnext2, ynext2, xprev2, yprev2, x1, y1, dx2, dy2, sx2, sy2, err2);

        int xmax = (xprev1 < xprev2) ? xprev2:xprev1;

        for (int x = (xprev1 < xprev2) ? xprev1:xprev2; x <= xmax; x++)
        {
            setPixel(x, y, 100, blue);
        }
    }
}

void Renderer::drawBLine(const QVector3D& point1, const QVector3D& point2, const QColor& color)
{
    int x1 = (int) point1.x();
    int y1 = (int) point1.y();
    int x2 = (int) point2.x();
    int y2 = (int) point2.y();

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1; // x increment
    int sy = (y1 < y2) ? 1 : -1; // y increment
    int err = dx - dy;

    while (true) {
        setPixel(x1, y1, 0, color);

        if ((x1 == x2) && (y1 == y2))
            break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void Renderer::initBLine(int x1, int y1, int x2, int y2, int& dx, int& dy, int& sx, int& sy, int& err)
{    
    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    sx = (x1 < x2) ? 1 : -1; // x increment
    sy = (y1 < y2) ? 1 : -1; // y increment
    err = dx - dy;
}

//void Renderer::getBLineNextPoint(const QVector2D& seg1start, QVector2D& seg1end, QVector2D& seg2start, const QVector2D& lineend, const int& dx, const int& dy, const int& sx, const int& sy, int& err)
//{
//    bool exit = false;
//    seg1end.setY(seg1start.y());
//    seg2start.setX(seg1start.x())
//    seg2start.setY(seg1start.y());

//    while (true) {
//        if (seg1start == lineend)
//            break;
//        int e2 = 2 * err;
//        if (e2 < dx) {
//            err += dx;
//            seg2start.setY(seg2start.y() + sy);
//            exit = true;
//        }
//        if (e2 > -dy) {
//            err -= dy;
//            if (exit) {
//                seg1end.setX(seg2start.x());
//            }
//            seg2start.setX(seg2start.x() + sx);
//        }
//        if (exit)
//            break;
//    }
//}

void Renderer::getBLineNextPoint(int& xnext, int& ynext, int& xprev, int& yprev, const int& xend, const int& yend, const int& dx, const int& dy, const int& sx, const int& sy, int& err)
{
    bool exit = false;
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
        for (QVector<int> face : mesh.m_faces) {
            QVector3D point1 = mesh.m_vertices[face[0]].project(view*world, projection, viewport);
            QVector3D point2 = mesh.m_vertices[face[1]].project(view*world, projection, viewport);
            QVector3D point3 = mesh.m_vertices[face[2]].project(view*world, projection, viewport);

            // triangle
            fillBTriangle(point1, point2, point3, mesh.getColor());

            // wireframe
            drawBLine(point1, point2, mesh.getColor());
            drawBLine(point2, point3, mesh.getColor());
            drawBLine(point3, point1, mesh.getColor());
        }
    }
}

