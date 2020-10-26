#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include "camera.h"
#include "renderer.h"
#include "mesh.h"

#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QThread>

class RenderLoop : public QObject
{
    Q_OBJECT

public:
    RenderLoop(Camera &camera, QVector<Mesh> &meshList, Renderer &renderer, QGraphicsPixmapItem &pixmap, QObject *parent = nullptr);
    ~RenderLoop();

public slots:
    void start();
    void stop();

signals:

private:
    // TODO change to const reference?
    // Model
    Camera& m_camera;
    QVector<Mesh>& m_meshList;

    // View
    Renderer& m_renderer;
    QGraphicsPixmapItem& m_pixmap;

    // Thread
    std::atomic<bool> m_abort;
};

#endif // RENDERTHREAD_H
