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
    m_image = m_backBuffer;
}

void Renderer::putPixel(int x, int y, const QColor& color)
{
    QColor toto = color;
    m_backBuffer.setPixel(x, y, toto.rgba());
}

void Renderer::drawPoint(const QVector2D& point, const QColor& color)
{
    //qDebug("Draw point at (%d, %d)", (int)point.x(), (int)point.y());
    if (point.x() >= 0 && point.y() >= 0 && point.x() < m_image.width() && point.y() < m_image.height()) {
        // Drawing a point
        putPixel((int)point.x(), (int)point.y(), color);
    }
}

void Renderer::drawLine(const QVector2D &point0, const QVector2D &point1, const QColor& color)
{
    float dist = (point1 - point0).length();
    if (dist < 1)
        return;
    QVector2D middlePoint = point0 + (point1 - point0)/2;
    drawPoint(middlePoint, color);
    drawLine(point0, middlePoint, color);
    drawLine(middlePoint, point1, color);
}

void Renderer::render(Camera camera, const QVector<Mesh>& meshList)
{
    QMatrix4x4 view;
    view.setToIdentity();
    view.lookAt(camera.getPosition(), camera.getTarget(), camera.getDirection());
    QMatrix4x4 projection;
    projection.setToIdentity();
//    qDebug("Image WxH %dx%d", m_image.width(), m_image.height());
//    qDebug("Image format %d", m_image.format());
    projection.perspective(45, (float)m_image.width()/(float)m_image.height(), 1.0, 1000.0);
    QRect viewport(0, 0, m_image.width(), m_image.height());

    // Bitmap origin
    //this->drawPoint(QVector3D(0,0,0), Qt::red);

//    qDebug("Number of meshes %d", meshList.size());

    for(const Mesh& mesh : meshList) {
        QMatrix4x4 model;
        QQuaternion quaternion;
        QVector2D point0, point1, point2;

        qDebug("Rotation (%d, %d, %d)", (int)mesh.getRotation().x(), (int)mesh.getRotation().y(), (int)mesh.getRotation().z());
        qDebug("Position (%d, %d, %d)", (int)mesh.getPosition().x(), (int)mesh.getPosition().y(), (int)mesh.getPosition().z());

        quaternion.normalize();
        quaternion.fromEulerAngles(mesh.getRotation().x(), mesh.getRotation().y(), mesh.getRotation().z());
        model.setToIdentity();
        model.scale(mesh.getScale());
        model.rotate(quaternion);
        model.translate(mesh.getPosition());

        // Points, edges
//        for (int i = 0; i < mesh.m_vertices.size()-1; ++i) {
//            qDebug("Vertex (%d, %d, %d)", (int)mesh.m_vertices.at(i).x(), (int)mesh.m_vertices.at(i).y(), (int)mesh.m_vertices.at(i).z());
//            qDebug("Vertex (%d, %d, %d)", (int)mesh.m_vertices.at(i+1).x(), (int)mesh.m_vertices.at(i+1).y(), (int)mesh.m_vertices.at(i+1).z());
//            qDebug("Vertex color %d %d %d", mesh.getColor().red(), mesh.getColor().green(), mesh.getColor().blue());
//            point0 = mesh.m_vertices.at(i).project(view*model, projection, viewport).toVector2D();
//            point1 = mesh.m_vertices.at(i+1).project(view*model, projection, viewport).toVector2D();
////            quaternion.normalize();
////            quaternion.fromEulerAngles(mesh.getRotation().x(), mesh.getRotation().y(), mesh.getRotation().z());
////            model.setToIdentity();
////            model.scale(mesh.getScale());
////            model.rotate(quaternion); // OK
////            model.translate(mesh.getPosition()); // OK
//            //this->drawPoint(point.project(view*model, projection, viewport), mesh.getColor());
//            drawLine(point0, point1, mesh.getColor());
//        }

        // Faces
        qDebug("Number of faces %d", mesh.m_faces.size());
        for (QVector<int> face : mesh.m_faces) {
//            qDebug("Vertex (%d, %d, %d)", (int)mesh.m_vertices[face[0]].x(), (int)mesh.m_vertices[face[0]].y(), (int)mesh.m_vertices[face[0]].z());
//            qDebug("Vertex (%d, %d, %d)", (int)mesh.m_vertices[face[1]].x(), (int)mesh.m_vertices[face[1]].y(), (int)mesh.m_vertices[face[1]].z());
//            qDebug("Vertex (%d, %d, %d)", (int)mesh.m_vertices[face[2]].x(), (int)mesh.m_vertices[face[2]].y(), (int)mesh.m_vertices[face[2]].z());
//            qDebug("Vertex color %d %d %d", mesh.getColor().red(), mesh.getColor().green(), mesh.getColor().blue());
            point0 = mesh.m_vertices[face[0]].project(view*model, projection, viewport).toVector2D();
            point1 = mesh.m_vertices[face[1]].project(view*model, projection, viewport).toVector2D();
            point2 = mesh.m_vertices[face[2]].project(view*model, projection, viewport).toVector2D();
            drawLine(point0, point1, mesh.getColor());
            drawLine(point1, point2, mesh.getColor());
            drawLine(point2, point0, mesh.getColor());
        }
    }
}

