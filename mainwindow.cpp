#include "loader.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

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
    p_camera->setPosition(QVector3D(10, -3, 10));
    p_camera->setDirection(QVector3D(0, 1, 0));
    Loader::load(m_meshList);

    // View
    // UI
    ui->setupUi(this);

    // scene, render
    p_scene = new QGraphicsScene(this);
    ui->mainView->setScene(p_scene);
    p_render = new Animation(*p_camera, m_meshList, this);
    p_scene->addItem(p_render);
}

MainWindow::~MainWindow()
{
    qDebug("~MainWindow");

    // Model
    delete p_camera;

    // View
    delete ui;
    delete p_render;
}
