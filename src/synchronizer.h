#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H

#include <memory>
#include <QAtomicInt>
#include <atomic>

class QMutex;
class QWaitCondition;
class QAtomicInt;
class Progress;

class Synchronizer
{
public:
    Synchronizer( );
    ~Synchronizer( );

    void sleep( );

    void pause( );
    bool paused( ) const;
    void resume( );

    void cancel( );
    bool canceled( ) const;

    void resetCancel( );
    void resetBarrier( );

    int operator ++( );
    int operator ++(int);

    int  getThreadNum() const;
    void setThreadNum(int thrnum);

    std::shared_ptr<Progress>  getProgress();

private:
    std::unique_ptr<QMutex>           p_mutex;
    std::unique_ptr<QWaitCondition>   p_condVar;
    std::shared_ptr<Progress>         p_progress;
    std::atomic_bool                  m_canceled;
    QAtomicInt                        m_barrier;
    bool                              m_pause{false};
    int                               m_thr_num{0};

};

#endif // SYNCHRONIZER_H
