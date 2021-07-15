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

private:    
    QImage* p_image;
    float* p_zBuffer; // TODO use unique_ptr?

    // Bresenham
    void fillBTriangle(std::vector<QVector3D>& triangle_s, const QColor& color);
    void drawBLine(const QVector3D& point1, const QVector3D& point2, const QColor& color);
    void initBLine(const QVector3D& v1, const QVector3D& v2, int& dx, int& dy, int& sx, int& sy, int& err);
    void getBLineNextPoint(int& xnext, int& ynext, int& xprev, int& yprev, const QVector3D& vend, const int& dx, const int& dy, const int& sx, const int& sy, int& err);
    void getBLineNextSegment(const QVector2D& seg1start, QVector2D& seg1end, QVector2D& seg2start, const QVector2D& lineend, const int& dx, const int& dy, const int& sx, const int& sy, int& err);

    // Other
    void fillScanLine(const int& x1, const float& z1_inv, const int& x2, const float& z2_inv, int& y, const QColor& color);
    void setPixel(const int& x, const int& y, const float& z_inv, const QColor& color);
};

#endif // RENDERER_H
