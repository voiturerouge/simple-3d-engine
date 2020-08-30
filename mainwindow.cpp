#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QGraphicsPixmapItem>
#include <QString>
#include <QtCore>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug("MainWindow");

    // UI
    p_scene = new QGraphicsScene(this);
    ui->setupUi(this);
    ui->graphicsView->setScene(p_scene);

    // Canvas
    // p_canvas = new Canvas(this);
    // setCentralWidget(p_canvas);

    // Meshes
    p_mesh = new Mesh();

    // X axis
    p_mesh->m_vertices.clear();
    p_mesh->m_vertices.append(QVector3D(0, 0, 0));
    p_mesh->m_vertices.append(QVector3D(1, 0, 0));
    p_mesh->m_faces.clear();
    p_mesh->m_faces.append({0, 1, 1});
    p_mesh->setRotation(QVector3D(0, 0, 0));
    p_mesh->setPosition(QVector3D(0, 0, 0));
    p_mesh->setScale(1);
    p_mesh->setColor(Qt::red);
    m_meshList.append(*p_mesh);

    // Y axis
    p_mesh->m_vertices.clear();
    p_mesh->m_vertices.append(QVector3D(0, 0, 0));
    p_mesh->m_vertices.append(QVector3D(0, 1, 0));
    p_mesh->m_faces.clear();
    p_mesh->m_faces.append({0, 1, 1});
    p_mesh->setRotation(QVector3D(0, 0, 0));
    p_mesh->setPosition(QVector3D(0, 0, 0));
    p_mesh->setScale(1);
    p_mesh->setColor(Qt::green);
    m_meshList.append(*p_mesh);

    // Z axis
    p_mesh->m_vertices.clear();
    p_mesh->m_vertices.append(QVector3D(0, 0, 0));
    p_mesh->m_vertices.append(QVector3D(0, 0, 1));
    p_mesh->m_faces.clear();
    p_mesh->m_faces.append({0, 1, 1});
    p_mesh->setRotation(QVector3D(0, 0, 0));
    p_mesh->setPosition(QVector3D(0, 0, 0));
    p_mesh->setScale(1);
    p_mesh->setColor(Qt::blue);
    m_meshList.append(*p_mesh);

    // Cube
    p_mesh->m_vertices.clear();
    p_mesh->m_vertices.append(QVector3D(-1, 1, 1));
    p_mesh->m_vertices.append(QVector3D(1, 1, 1));
    p_mesh->m_vertices.append(QVector3D(-1, -1, 1));
    p_mesh->m_vertices.append(QVector3D(1, -1, 1));
    p_mesh->m_vertices.append(QVector3D(-1, 1, -1));
    p_mesh->m_vertices.append(QVector3D(1, 1, -1));
    p_mesh->m_vertices.append(QVector3D(1, -1, -1));
    p_mesh->m_vertices.append(QVector3D(-1, -1, -1));
    p_mesh->m_faces.clear();
    p_mesh->m_faces.append({0, 1, 2});
    p_mesh->m_faces.append({1, 2, 3});
    p_mesh->m_faces.append({1, 3, 6});
    p_mesh->m_faces.append({1, 5, 6});
    p_mesh->m_faces.append({0, 1, 4});
    p_mesh->m_faces.append({1, 4, 5});
    p_mesh->m_faces.append({2, 3, 7});
    p_mesh->m_faces.append({3, 6, 7});
    p_mesh->m_faces.append({0, 2, 7});
    p_mesh->m_faces.append({0, 4, 7});
    p_mesh->m_faces.append({4, 5, 6});
    p_mesh->m_faces.append({4, 6, 7});
    p_mesh->setRotation(QVector3D(-143, 30, 30));
    p_mesh->setPosition(QVector3D(0, 0, 0));
    p_mesh->setScale(2);
    p_mesh->setColor(Qt::yellow);
    m_meshList.append(*p_mesh);

    // Camera
    p_camera = new Camera();
    p_camera->setTarget(QVector3D(0, 0, 0));
    p_camera->setPosition(QVector3D(10, 3, 5));
    p_camera->setDirection(QVector3D(0.5, -1, 0));

    // Image
    p_image = new QImage(640, 480, QImage::Format_ARGB32);
    //p_image = new QImage(":/images/Freedo.bmp");
    if (p_image->isNull())
        qDebug("Image did not load properly!");
    qDebug("Image format %d", p_image->format());

    // Device
    p_device = new Device();
    p_device->setImage(*p_image);
    p_device->clear(Qt::black);
    //std::initializer_list<Mesh> meshes = {*p_mesh};
    p_device->render(*p_camera, m_meshList);
    p_device->present();

    //
    p_pixmapItem = new QGraphicsPixmapItem();
    p_pixmapItem->setPixmap(QPixmap::fromImage(p_device->getImage()));
    p_scene->addItem(p_pixmapItem);
    ui->graphicsView->show();

    // Start the render thread
    //m_renderThread.start();
}

MainWindow::~MainWindow()
{
    qDebug("~MainWindow");
    delete ui;
    delete p_camera;
    delete p_device;
    delete p_mesh;
    delete p_image;
    delete p_scene;
}
