#include "mesh.h"

Mesh::Mesh()
{
    m_position = QVector3D(0, 0, 0);
    m_rotation = QVector3D(0, 0, 0);
}

QString Mesh::getName()
{
    return m_name;
}

void Mesh::setName(const QString& name)
{
    m_name = name;
}

void Mesh::setPosition(const QVector3D& position)
{
    m_position = position;
}

QVector3D Mesh::getPosition() const
{
    return m_position;
}

void Mesh::setRotation(const QVector3D& rotation)
{
    m_rotation = rotation;
}

QVector3D Mesh::getRotation() const
{
    return m_rotation;
}

void Mesh::setScale(float scale)
{
    m_scale = scale;
}

float Mesh::getScale() const
{
    return m_scale;
}

void Mesh::setColor(const QColor& color)
{
    m_color = color;
}

QColor Mesh::getColor() const
{
    return m_color;
}
