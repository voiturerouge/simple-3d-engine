#include "renderer.h"

Renderer::Renderer()
{

}

void Renderer::setImage(QImage image)
{
    // Check image format
    if (!image.hasAlphaChannel())
        throw std::invalid_argument("Invalid image format");

    m_image = image;
    m_backBuffer = image;
}

QImage Renderer::getImage() const
{
    return m_image;
}

void Renderer::clear(const QColor& color)
{
    m_backBuffer.fill(color);
}

void Renderer::present()
{
    // Write the back buffer to the image
    m_image = m_backBuffer.copy();
}

void Renderer::putPixel(int x, int y, const QColor& color)
{
    m_backBuffer.setPixel(x, y, color.rgba());
}

void Renderer::drawPoint(const QVector2D& point, const QColor& color)
{
    //qDebug("Draw point at (%d, %d)", (int)point.x(), (int)point.y());
    if (point.x() >= 0 && point.y() >= 0 && point.x() < m_image.width() && point.y() < m_image.height()) {
        // Drawing a point
        putPixel((int)point.x(), (int)point.y(), color);
    }
}

void Renderer::drawLine(const QVector2D& point0, const QVector2D& point1, const QColor& color)
{
    float dist = (point1 - point0).length();
    if (dist < 1)
        return;
    QVector2D middlePoint = point0 + (point1 - point0)/2;
    drawPoint(middlePoint, color);
    drawLine(point0, middlePoint, color);
    drawLine(middlePoint, point1, color);
}

void Renderer::render(const Camera& camera, const QVector<Mesh>& meshList)
{
    // View
    QMatrix4x4 view;
    view.setToIdentity();
    view.lookAt(camera.getPosition(), camera.getTarget(), camera.getDirection());

    // Projection
    QMatrix4x4 projection;
    projection.setToIdentity();
    projection.perspective(45, (float)m_image.width()/(float)m_image.height(), 1.0, 1000.0);
    QRect viewport(0, 0, m_image.width(), m_image.height());

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
            QVector2D point0 = mesh.m_vertices[face[0]].project(view*world, projection, viewport).toVector2D();
            QVector2D point1 = mesh.m_vertices[face[1]].project(view*world, projection, viewport).toVector2D();
            QVector2D point2 = mesh.m_vertices[face[2]].project(view*world, projection, viewport).toVector2D();
            drawLine(point0, point1, mesh.getColor());
            drawLine(point1, point2, mesh.getColor());
            drawLine(point2, point0, mesh.getColor());
        }
    }
}

