#include "renderloop.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

RenderLoop::RenderLoop(Camera &camera, QVector<Mesh> &meshList, Renderer &renderer, QGraphicsPixmapItem &pixmap, QObject *parent)
    : QObject(parent)
    , m_camera(camera)
    , m_meshList(meshList)
    , m_renderer(renderer)
    , m_pixmap(pixmap)
{
    // TODO assert on all pointers
    qDebug("RenderLoop");
    m_abort = false;
}

RenderLoop::~RenderLoop()
{
    qDebug("~RenderLoop");
}

void RenderLoop::start()
{
    qDebug("RenderLoop::start");
    int i = 0;
    QVector3D rotation(0, 0, 0);

//    m_meshList[0].setPosition(QVector3D(-12,0,0));
//    m_renderer.clear(Qt::black); // TODO to be put directly in the renderer
//    m_renderer.render(m_camera, m_meshList);
//    m_renderer.present();
//    m_pixmap.setPixmap(QPixmap::fromImage(m_renderer.getImage()));


    while (!m_abort) {
        qDebug("%d", i);
        i++;

        //m_meshList[0].setPosition(QVector3D(-(i%12),0,0));
//        m_renderer.clear(Qt::black); // A mettre dans le device
//        m_renderer.render(m_camera, m_meshList);
//        m_renderer.present();
//        m_pixmap.setPixmap(QPixmap::fromImage(m_renderer.getImage()));

//        // Rotate
//        rotation.setX(i);
//        m_meshList[0].setPosition(QVector3D(i, 0, 0));

//        // Render and present
//        m_device.clear(Qt::black); // A mettre dans le device
//        m_device.render(m_camera, m_meshList);
//        m_device.present();

//        // Update pixmap
//        m_pixmap.setPixmap(QPixmap::fromImage(m_device.getImage()));
////        p_scene->addItem(p_pixmapItem);
////        ui->graphicsView->show();


////        p_pixmapItem->setPixmap(QPixmap::fromImage(p_device->getImage()));
////        ui->graphicsView->show();

        // Sleep
        usleep(1000000);
    }
    qDebug("Exiting loop!");
}

void RenderLoop::stop()
{
    qDebug("RenderLoop::stop");
    m_abort = true;
}
