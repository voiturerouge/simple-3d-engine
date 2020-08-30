#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QObject>

class renderThread : public QThread
{
    Q_OBJECT

public:
    renderThread(QObject *parent = nullptr);
    ~renderThread();

signals:

protected:
    void run() override;

private:
    QMutex m_mutex;
    QWaitCondition m_condition;
    bool m_abort;
};

#endif // RENDERTHREAD_H
