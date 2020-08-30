#ifndef MESH_H
#define MESH_H

#include <QColor>
#include <QVector>
#include <QVector3D>
#include <QObject>

class Mesh
{
public:
    Mesh();

    QString getName();
    void setName(const QString& name);
    void setPosition(const QVector3D& position);
    QVector3D getPosition() const;
    void setRotation(const QVector3D& rotation);
    QVector3D getRotation() const;
    void setScale(float scale);
    float getScale() const;
    void setColor(const QColor&);
    QColor getColor() const;

    QVector<QVector3D> m_vertices;
    QVector<QVector<int>> m_faces;

private:
    QString m_name;
    QVector3D m_position;
    QVector3D m_rotation;
    float m_scale;
    QColor m_color;

    void setVertices(QVector<QVector3D> *vertices);
};

#endif // MESH_H
