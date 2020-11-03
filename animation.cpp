#include "animation.h"
#include "math.h"

Animation::Animation(Camera &camera, QVector<Mesh> &meshList, QObject *parent)
    : QObject(parent)
    , m_camera(camera)
    , m_meshList(meshList)
{
    // renderer
    p_renderer = new Renderer(640, 480, QImage::Format_ARGB32);
    p_renderer->clear(Qt::black);
    m_pixmap = QPixmap::fromImage(p_renderer->getImage()).copy();
    this->update(0, 0, 640, 480);

    p_timer = new QTimer();
    connect(p_timer, &QTimer::timeout, this, &Animation::nextFrame);
    p_timer->start(25);
}

void Animation::nextFrame()
{
    static float i = 0;
    i = i + 0.5f;
    m_meshList[3].setRotation(QVector3D(fmod(i,360), fmod(i,360), 0));
    //m_meshList[3].setPosition(QVector3D(-1,0,0));

    p_renderer->clear(Qt::black); // TODO to be put directly in the renderer
    p_renderer->render(m_camera, m_meshList);
    m_pixmap = QPixmap::fromImage(p_renderer->getImage()).copy();
    this->update(0, 0, 640, 480);
}

void Animation::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->drawPixmap(0, 0, m_pixmap);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF Animation::boundingRect() const
{
    return QRectF(0, 0, 640, 480);
}
