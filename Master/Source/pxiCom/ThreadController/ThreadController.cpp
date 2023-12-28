#include "ThreadController.h"

ThreadController::ThreadController(QObject *parent,  sharedWork work) :
    QObject(parent)
{
    m_work = sharedWork::create();
    m_work =  work;

    work->moveToThread(&m_thread);
//    connect(&m_thread, SIGNAL(finished()), aecWork.data(), SLOT(deleteLater()));  // 智能指针，自动删内存
//    connect(&m_thread, SIGNAL(Finish()), aecWork.data(), SLOT(OnFinish()));
    connect(&m_thread, SIGNAL(started()), m_work.data(), SLOT(Run()));

    m_thread.start();
}

ThreadController::~ThreadController()
{
//    emit Finish();        // 不能用信号，不能保证aecWork内存还在
    m_work->OnFinish();
    m_thread.quit();
    m_thread.wait();
}

/*!
 * \brief ThreadController::退出
 */
void ThreadController::Quit()
{
    delete this;
}
