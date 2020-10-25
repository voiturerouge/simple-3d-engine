#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "camera.h"
#include "device.h"
#include "mesh.h"
#include "renderthread.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

private slots:

private:
    Ui::MainWindow* ui;
    RenderThread* p_renderThread;
    Camera* p_camera;
    QImage* p_image;
    Device* p_device;
    Mesh* p_mesh;
    QGraphicsPixmapItem* p_pixmap;
    QGraphicsScene* p_scene;

    // Mesh
    QVector<Mesh> m_meshList;

};
#endif // MAINWINDOW_H
