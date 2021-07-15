#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QObject>

class Camera
{
public:
    Camera();
    void setPosition(const QVector3D& position);
    QVector3D getPosition() const;
    void setTarget(const QVector3D& target);
    QVector3D getTarget() const;
    void setDirection(const QVector3D& direction);
    QVector3D getDirection() const;

private:
    QVector3D m_position;
    QVector3D m_target;
    QVector3D m_direction;
};

#endif // CAMERA_H
