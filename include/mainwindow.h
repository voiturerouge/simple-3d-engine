#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "camera.h"
#include "renderer.h"
#include "mesh.h"
#include "animation.h"

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
    // Model
    Camera* p_camera;
    QVector<Mesh> m_meshList;

    // View
    Ui::MainWindow* ui;
    QGraphicsScene* p_scene;
    Animation* p_render;
};
#endif // MAINWINDOW_H
