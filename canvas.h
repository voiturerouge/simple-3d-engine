#ifndef CANVAS_H
#define CANVAS_H

#include <QImage>
#include <QWidget>


class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);
    void setImage(const QImage& image);

protected:
    void paintEvent(QPaintEvent* event);

signals:

private:
    QImage m_image;
};

#endif // CANVAS_H
