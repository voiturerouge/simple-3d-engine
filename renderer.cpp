#include "renderer.h"

Renderer::Renderer(int width, int height, QImage::Format format)
{
    p_image = new QImage(width, height, format);
}

Renderer::~Renderer()
{
    delete p_image;
}

QImage Renderer::getImage() const
{
    return *p_image;
}

void Renderer::clear(const QColor& color)
{
    p_image->fill(color);
}

void Renderer::putPixel(int x, int y, const QColor& color)
{
    // TODO setPixel is expensive, use the scanline array instead
    p_image->setPixel(x, y, color.rgba());
}

void Renderer::drawPoint(const QVector2D& point, const QColor& color)
{
    if (point.x() >= 0 && point.y() >= 0 && point.x() < p_image->width() && point.y() < p_image->height()) {
        putPixel((int)point.x(), (int)point.y(), color);
    }
}


void Renderer::drawLine(const QVector2D& point1, const QVector2D& point2, const QColor& color)
{
    float dist = (point2 - point1).length();
    if (dist < 1)
        return;
    QVector2D middlePoint = point1 + (point2 - point1)/2;
    drawPoint(middlePoint, color);
    drawLine(point1, middlePoint, color);
    drawLine(middlePoint, point2, color);
}

void Renderer::drawBTriangle(const QVector2D& point1, const QVector2D& point2, const QVector2D& point3, const QColor& color)
{
    // sort vertices
    std::vector<std::vector<int>> points{{(int) point1.x(), (int) point1.y()},
                                         {(int) point2.x(), (int) point2.y()},
                                         {(int) point3.x(), (int) point3.y()}};
    std::sort(points.begin(),
              points.end(),
              [](std::vector<int> pointa, std::vector<int> pointb) { return pointa[1] < pointb[1]; });

    int x1 = points[0][0];
    int y1 = points[0][1];
    int x2 = points[1][0];
    int y2 = points[1][1];
    int x3 = points[2][0];
    int y3 = points[3][1];

    int err1;
    int dx1;
    int dy1;
    int err2;
    int dx2;
    int dy2;

    // fill top triangle
    // line point1 to point2
    dx1 = err1 * 2;
    dy1 = (y2-y1) * 2;
    err1 = x2-x1;

    // line point1 to point3
    err2 = x3-x1;
    dx2 = err2 * 2;
    dy2 = (y3-y1) * 2;

    for (int y=y1; y<=y2; y++) {
        getNextBPoint();
    }


    // fill bottom triangle
    // line point3 to point1
    err1 = x3-x1;
    dx1 = err1 * 2;
    dy1 = (y3-y1) * 2;

    // line point3 to point2
    err2 = x3-x2;
    dx2 = err2 * 2;
    dy2 = (y3-y2) * 2;

    for (int y=y3; y<=y2; y--) {
        getNextBPoint();
    }
}

void Renderer::drawBLine(const QVector2D& point1, const QVector2D& point2, const QColor& color)
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
        putPixel(x1, y1, color);

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

void initBLine(int x1, int y1, int x2, int y2, int& dx, int& dy, int& err)
{    
    dx = x2 - x1;
    dy = y2 - y1;
    int inc;


    if (dx > 0) {
        if (dy > 0) {
            // 0 < m < 1
            if (dx >= dy) {
                err = dx;
                dx = dx * 2;
                dy = dy * 2;
            }
            //  m > 1
            else {
                err = dy;
                dx = dx * 2;
                dy = dy * 2;
             }
        }
        else {
            // -1 < m < 0
            if (dx >= -dy) {
                err = dx;
                dx = dx * 2;
                dy = dy * 2;
            }
            //  m < -1
            else {
                err = dy;
                dx = dx * 2;
                dy = dy * 2;
            }
        }
    }
    else {
        if (dy > 0) {
            // -1 < m < 0
            if (-dx >= dy) {
                err = dx;
                dx = dx * 2;
                dy = dy * 2;
            }
            //  m < -1
            else {
                err = dy;
                dx = dx * 2;
                dy = dy * 2;
            }
        }
        else {
            // 0 < m < 1
            if (dx <= dy) {
                err = dx;
                dx = dx * 2;
                dy = dy * 2;
            }
            //  m > 1
            else {
                err = dy;
                dx = dx * 2;
                dy = dy * 2;
            }
        }
    }

    if (abs(dy) < abs(dx))
    {
        // loop on x

        // determine error and increment
        if (dx)
        {
            // 0 < m < 1
            if (dy)
            {
                err = dx;
                dx = dx * 2;
                dy = dy * 2;
            }
            // -1 < m < 0
            else
            {
                err = dx;
                dx = dx * 2;
                dy = dy * 2;
            }
        }
        else
        {
            // -1 < m < 0
            if (dy)
            {
                err = dx;
                dx = dx * 2;
                dy = dy * 2;
            }
            // 0 < m < 1
            else
            {
                err = dx;
                dx = dx * 2;
                dy = dy * 2;
            }
        }
    }
    else
    {
        // loop on y

        // determine error and increment
        if (dx)
        {
            //  m > 1
            if (dy)
            {
                err = dy;
                dx = dx * 2;
                dy = dy * 2;
            }
            //  m < -1
            else
            {
                err = dy;
                dx = dx * 2;
                dy = dy * 2;
            }
        }
        else
        {
            //  m < -1
            if (dy)
            {
                err = dy;
                dx = dx * 2;
                dy = dy * 2;
            }
            //  m > 1
            else
            {
                err = dy;
                dx = dx * 2;
                dy = dy * 2;
            }
        }
    }
}

void Renderer::getNextBPoint()
{
    //err = err + dx;

    // dx

    int dx;
    int dy;



    if (err < 0) {

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
            QVector2D point1 = mesh.m_vertices[face[0]].project(view*world, projection, viewport).toVector2D();
            QVector2D point2 = mesh.m_vertices[face[1]].project(view*world, projection, viewport).toVector2D();
            QVector2D point3 = mesh.m_vertices[face[2]].project(view*world, projection, viewport).toVector2D();

            drawTriangle(point1, point2, point3, mesh.getColor());

            // wireframe
//            drawLine(point1, point2, mesh.getColor());
//            drawLine(point2, point3, mesh.getColor());
//            drawLine(point3, point1, mesh.getColor());
//            drawBLine(point1, point2, mesh.getColor());
//            drawBLine(point2, point3, mesh.getColor());
//            drawBLine(point3, point1, mesh.getColor());
        }
    }
}

