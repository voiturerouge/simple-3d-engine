#include "loader.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QGraphicsPixmapItem>
#include <QString>
#include <QtCore>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug("MainWindow");

    // Model
    p_camera = new Camera();
    p_camera->setTarget(QVector3D(0, 0, 0));
    p_camera->setPosition(QVector3D(10, 3, 5));
    p_camera->setDirection(QVector3D(0, -1, 0));
    Loader::load(m_meshList);

    // View
    // UI
    ui->setupUi(this);

    // scene
    p_scene = new QGraphicsScene(this);
    ui->mainView->setScene(p_scene);

    // renderer
    p_image = new QImage(640, 480, QImage::Format_ARGB32);
    p_renderer = new Renderer();
    p_renderer->setImage(*p_image);
    p_renderer->clear(Qt::black);
    p_renderer->render(*p_camera, m_meshList);
    p_renderer->present();

    // pixmap
    p_pixmap = new QGraphicsPixmapItem();
    p_pixmap->setPixmap(QPixmap::fromImage(p_renderer->getImage()));
    p_scene->addItem(p_pixmap);
    ui->mainView->update();

    // Thread
    p_thread = new QThread;
    p_renderLoop = new RenderLoop(*p_camera, m_meshList, *p_renderer, *p_pixmap);
    p_renderLoop->moveToThread(p_thread);
    connect(p_thread, &QThread::started, p_renderLoop, &RenderLoop::start);
    connect(p_thread, &QThread::finished, p_thread, &QObject::deleteLater);
    p_thread->start();
}

MainWindow::~MainWindow()
{
    qDebug("~MainWindow");

    // Thread
    p_renderLoop->stop();
    p_thread->quit();
    p_thread->wait();
    delete p_renderLoop;
    delete p_thread;

    // Model
    delete p_camera;
    delete p_renderer;

    // View
    delete ui;
    delete p_image;
    delete p_scene;
}
