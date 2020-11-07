#ifndef RENDERER_H
#define RENDERER_H

#include "camera.h"
#include "mesh.h"

#include <QColor>
#include <QImage>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>
#include <QObject>

class Renderer
{
public:
    Renderer(int width, int height, QImage::Format format);
    ~Renderer();

    void clear(const QColor& color);
    void render(const Camera& camera, const QVector<Mesh>& meshList);
    QImage getImage() const;

    void drawTriangle(const QVector2D& point1, const QVector2D& point2, const QVector2D& point3, const QColor& color);
    void drawLine(const QVector2D& point1, const QVector2D& point2, const QColor& color);
    void drawPoint(const QVector2D& point, const QColor& color);
    void putPixel(int x, int y, const QColor& color);

    // TODO Bresenham
    void drawBTriangle(const QVector2D& point1, const QVector2D& point2, const QVector2D& point3, const QColor& color);
    void drawBLine(const QVector2D& point1, const QVector2D& point2, const QColor& color);
    void initBLine(int x1, int y1, int x2, int y2, int& dx, int& dy, int& err);
    void getNextBPoint(int currx, int& dx, int& dy, int& err, int increment, int sign, );

private:    
    QImage* p_image;
};

#endif // RENDERER_H
