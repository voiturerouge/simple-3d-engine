#ifndef MODELLOOP_H
#define MODELLOOP_H

#include "camera.h"
#include "renderer.h"
#include "mesh.h"

#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QThread>

class ModelLoop : public QObject
{
    Q_OBJECT

public:
    ModelLoop(Camera &camera, QVector<Mesh> &meshList, QObject *parent = nullptr);
    ~ModelLoop();

public slots:
    void start();
    void stop();

signals:

private:
    // TODO change to const reference?
    // Model
    Camera& m_camera;
    QVector<Mesh>& m_meshList;

    // Thread
    std::atomic<bool> m_abort;
};

#endif // MODELLOOP_H
