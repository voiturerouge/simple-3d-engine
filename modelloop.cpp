#include "modelloop.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

ModelLoop::ModelLoop(Camera &camera, QVector<Mesh> &meshList, QObject *parent)
    : QObject(parent)
    , m_camera(camera)
    , m_meshList(meshList)
{
    // TODO assert all pointers
    qDebug("RenderLoop");
    m_abort = false;
}

ModelLoop::~ModelLoop()
{
    qDebug("~ModelLoop");
}

void ModelLoop::start()
{
    qDebug("ModelLoop::start");
    int i = 0;
    while (!m_abort) {
        qDebug("%d", i);
        i++;

        // Move
        m_meshList[3].setPosition(QVector3D(-(i%12),0,0));

        // Sleep
        usleep(2000000);
    }
    qDebug("Exiting loop!");
}

void ModelLoop::stop()
{
    qDebug("ModelLoop::stop");
    m_abort = true;
}
