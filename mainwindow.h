#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "canvas.h"
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
    renderThread m_renderThread; // The render thread is allocated on the stack
    Canvas* p_canvas;
    Camera* p_camera;
    QImage* p_image;
    Device* p_device;
    Mesh* p_mesh;
    QGraphicsPixmapItem* p_pixmapItem;
    QGraphicsScene* p_scene;

    QVector<Mesh> m_meshList;


};
#endif // MAINWINDOW_H
