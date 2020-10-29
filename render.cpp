#include "render.h"
#include "math.h"

Render::Render(Camera &camera, QVector<Mesh> &meshList, QObject *parent)
    : QObject(parent)
    , m_camera(camera)
    , m_meshList(meshList)
{
    // renderer
    p_image = new QImage(640, 480, QImage::Format_ARGB32);
    p_renderer = new Renderer();
    p_renderer->setImage(*p_image); // TODO move width, height, format to the constructor
    p_renderer->clear(Qt::black);
    p_renderer->present();
    m_pixmap = QPixmap::fromImage(p_renderer->getImage()).copy();
    this->update(0, 0, 640, 480);

    p_timer = new QTimer();
    connect(p_timer, &QTimer::timeout, this, &Render::nextFrame);
    p_timer->start(25);
}

void Render::nextFrame()
{
    static float i = 0;
    i = i + 0.5f;
    m_meshList[3].setRotation(QVector3D(fmod(i,360), fmod(i,360), 0));
    //m_meshList[3].setPosition(QVector3D(-1,0,0));

    p_renderer->clear(Qt::black); // TODO to be put directly in the renderer
    p_renderer->render(m_camera, m_meshList);
    p_renderer->present();
    m_pixmap = QPixmap::fromImage(p_renderer->getImage()).copy();
    this->update(0, 0, 640, 480);
}

void Render::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->drawPixmap(0, 0, m_pixmap);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF Render::boundingRect() const
{
    return QRectF(0, 0, 640, 480);
}
