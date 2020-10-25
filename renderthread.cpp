#include "renderthread.h"

RenderThread::RenderThread(Camera &camera, QVector<Mesh> &meshList, Device &device, QGraphicsPixmapItem &pixmap, QObject *parent)
    : QThread(parent)
    , p_camera(camera)
    , p_meshList(meshList)
    , p_device(device)
    , p_pixmapItem(pixmap)
{
    qDebug("renderThread");
    m_abort = false;
}

RenderThread::~RenderThread()
{
    qDebug("~renderThread in");
    m_abort = true;
    wait();
    qDebug("~renderThread out");
}

void RenderThread::run()
{
    qDebug("renderThread::run");
    int i = 0;
    while (!m_abort) {
        qDebug("render thread %d", i);
        i++;
//        // Rotate
//        p_meshList[0].setRotation();

//        // Render and present
//        p_device->render(*p_camera, *p_meshList);
//        p_device->present();

//        // Show image
//        //
//        QGraphicsPixmapItem pixmapItem;
//        pixmapItem.setPixmap(QPixmap::fromImage(p_device->getImage()));
//        p_scene->addItem(p_pixmapItem);
//        ui->graphicsView->show();


//        p_pixmapItem->setPixmap(QPixmap::fromImage(p_device->getImage()));
//        ui->graphicsView->show();

        // Sleep
        usleep(100000);
    }
    qDebug("Exiting thread loop!");
    qDebug() << "thread id " << QThread::currentThreadId();
}
