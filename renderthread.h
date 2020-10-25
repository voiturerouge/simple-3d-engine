#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include "camera.h"
#include "device.h"
#include "mesh.h"

#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QThread>
#include <QWaitCondition>


class RenderThread : public QThread
{
    Q_OBJECT

public:
    RenderThread(Camera &camera, QVector<Mesh> &meshList, Device &device, QGraphicsPixmapItem &pixmap, QObject *parent = nullptr);
    ~RenderThread();

signals:

protected:
    void run() override;

private:
    // TODO change to const reference?
    std::atomic<bool> m_abort;
    Camera& p_camera;
    QVector<Mesh>& p_meshList;
    Device& p_device;
    QGraphicsPixmapItem& p_pixmapItem;
};

#endif // RENDERTHREAD_H
