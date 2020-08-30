#include "canvas.h"

#include <QPainter>

Canvas::Canvas(QWidget *parent) : QWidget(parent)
{

}

void Canvas::setImage(const QImage& image)
{
    m_image = image;
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(QPointF(0,0), m_image);
}

