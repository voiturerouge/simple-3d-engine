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
    QMatrix4x4 view;
    view.setToIdentity();
    view.lookAt(camera.getPosition(), camera.getTarget(), camera.getDirection());

    QMatrix4x4 projection;
    projection.setToIdentity();
    projection.perspective(45, (float)m_image.width()/(float)m_image.height(), 1.0, 1000.0);
    QRect viewport(0, 0, m_image.width(), m_image.height());

//    qDebug("Number of meshes %d", meshList.size());

    for(const Mesh& mesh : meshList) {
        QMatrix4x4 world;
        QQuaternion quaternion;
        QVector2D point0, point1, point2;

//        qDebug("Rotation (%d, %d, %d)", (int)mesh.getRotation().x(), (int)mesh.getRotation().y(), (int)mesh.getRotation().z());
//        qDebug("Position (%d, %d, %d)", (int)mesh.getPosition().x(), (int)mesh.getPosition().y(), (int)mesh.getPosition().z());

        quaternion.normalize();
        quaternion.fromEulerAngles(mesh.getRotation().x(), mesh.getRotation().y(), mesh.getRotation().z());
        world.setToIdentity();
        world.scale(mesh.getScale());
        world.rotate(quaternion);
        world.translate(mesh.getPosition());

        // Faces
//        qDebug("Number of faces %d", mesh.m_faces.size());
        for (QVector<int> face : mesh.m_faces) {
            point0 = mesh.m_vertices[face[0]].project(view*world, projection, viewport).toVector2D();
            point1 = mesh.m_vertices[face[1]].project(view*world, projection, viewport).toVector2D();
            point2 = mesh.m_vertices[face[2]].project(view*world, projection, viewport).toVector2D();
            drawLine(point0, point1, mesh.getColor());
            drawLine(point1, point2, mesh.getColor());
            drawLine(point2, point0, mesh.getColor());
        }
    }
}

