#include "camera.h"

Camera::Camera()
{
    m_direction = QVector3D(0,1,0);
}

void Camera::setPosition(const QVector3D& position)
{
    m_position = position;
}

QVector3D Camera::getPosition() const
{
    return m_position;
}

void Camera::setTarget(const QVector3D& target)
{
    m_target = target;
}

QVector3D Camera::getTarget() const
{
    return m_target;
}

void Camera::setDirection(const QVector3D& direction)
{
    m_direction = direction;
}

QVector3D Camera::getDirection() const
{
    return m_direction;
}
