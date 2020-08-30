#include "renderthread.h"

renderThread::renderThread(QObject *parent)
    : QThread(parent)
{
    qDebug("renderThread");
    m_abort = false;
}

renderThread::~renderThread()
{
    qDebug("~renderThread");
    m_abort = true;
    m_mutex.lock();
    m_condition.wakeAll();
    m_mutex.unlock();
    wait();
}

void renderThread::run()
{
    forever {
        m_mutex.lock();
        m_condition.wait(&m_mutex);
        m_mutex.unlock();

        int i = 0;
        while (!m_abort) {
            qDebug("worker thread %d", i);
            i++;
            usleep(100000);
        }

        if (m_abort)
            return;
    }
    qDebug("Exiting thread loop!");

}
