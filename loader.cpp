#include "loader.h"

Loader::Loader()
{

}

void Loader::load(QVector<Mesh>& meshList)
{
    Mesh mesh;

    // X axis
    mesh.m_vertices.clear();
    mesh.m_vertices.append(QVector3D(0, 0, 0));
    mesh.m_vertices.append(QVector3D(1, 0, 0));
    mesh.m_faces.clear();
    mesh.m_faces.append({0, 1, 1});
    mesh.setRotation(QVector3D(0, 0, 0));
    mesh.setPosition(QVector3D(0, 0, 0));
    mesh.setScale(1);
    mesh.setColor(Qt::red);
    meshList.append(mesh);

    // Y axis
    mesh.m_vertices.clear();
    mesh.m_vertices.append(QVector3D(0, 0, 0));
    mesh.m_vertices.append(QVector3D(0, 1, 0));
    mesh.m_faces.clear();
    mesh.m_faces.append({0, 1, 1});
    mesh.setRotation(QVector3D(0, 0, 0));
    mesh.setPosition(QVector3D(0, 0, 0));
    mesh.setScale(1);
    mesh.setColor(Qt::green);
    meshList.append(mesh);

    // Z axis
    mesh.m_vertices.clear();
    mesh.m_vertices.append(QVector3D(0, 0, 0));
    mesh.m_vertices.append(QVector3D(0, 0, 1));
    mesh.m_faces.clear();
    mesh.m_faces.append({0, 1, 1});
    mesh.setRotation(QVector3D(0, 0, 0));
    mesh.setPosition(QVector3D(0, 0, 0));
    mesh.setScale(1);
    mesh.setColor(Qt::blue);
    meshList.append(mesh);

    // Cube
    mesh.m_vertices.clear();
    mesh.m_vertices.append(QVector3D(-1, 1, 1));
    mesh.m_vertices.append(QVector3D(1, 1, 1));
    mesh.m_vertices.append(QVector3D(-1, -1, 1));
    mesh.m_vertices.append(QVector3D(1, -1, 1));
    mesh.m_vertices.append(QVector3D(-1, 1, -1));
    mesh.m_vertices.append(QVector3D(1, 1, -1));
    mesh.m_vertices.append(QVector3D(1, -1, -1));
    mesh.m_vertices.append(QVector3D(-1, -1, -1));
    mesh.m_faces.clear();
    mesh.m_faces.append({0, 1, 2});
    mesh.m_faces.append({1, 2, 3});
    mesh.m_faces.append({1, 3, 6});
    mesh.m_faces.append({1, 5, 6});
    mesh.m_faces.append({0, 1, 4});
    mesh.m_faces.append({1, 4, 5});
    mesh.m_faces.append({2, 3, 7});
    mesh.m_faces.append({3, 6, 7});
    mesh.m_faces.append({0, 2, 7});
    mesh.m_faces.append({0, 4, 7});
    mesh.m_faces.append({4, 5, 6});
    mesh.m_faces.append({4, 6, 7});
    mesh.setRotation(QVector3D(-90, 90, 123));
    mesh.setPosition(QVector3D(0, 0, 0));
    mesh.setScale(2);
    mesh.setColor(Qt::yellow);
    meshList.append(mesh);
}

