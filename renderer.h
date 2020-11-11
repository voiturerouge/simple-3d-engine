#ifndef RENDERER_H
#define RENDERER_H

#include "camera.h"
#include "mesh.h"

#include <cerrno>

#include <QColor>
#include <QImage>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>
#include <QObject>

class Renderer
{
public:
    Renderer(int width, int height);
    ~Renderer();

    void clear(const QColor& color);
    void render(const Camera& camera, const QVector<Mesh>& meshList);
    QImage getImage() const;
    void setPixel(const int& x, const int& y, const float& z, const QColor& color);

    // Bresenham
    void fillBTriangle(const QVector3D& point1, const QVector3D& point2, const QVector3D& point3, const QColor& color);
    void drawBLine(const QVector3D& point1, const QVector3D& point2, const QColor& color);
    void initBLine(int x1, int y1, int x2, int y2, int& dx, int& dy, int& sx, int& sy, int& err);
    void getBLineNextPoint(int& xnext, int& ynext, int& xprev, int& yprev, const int& xend, const int& yend, const int& dx, const int& dy, const int& sx, const int& sy, int& err);


private:    
    QImage* p_image;
    float* p_zBuffer; // TODO use unique_ptr?
};

#endif // RENDERER_H
