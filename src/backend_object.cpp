#include "backend_object.h"
#include "graphs_data.h"
#include "models.h"
#include "synchronizer.h"
#include "system_data.h"
#include "table_data.h"
#include "worker_object.h"
#include "worker_table.h"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QTabWidget>
#include <QTableView>
#include <QThread>

Backend_Object::Backend_Object(QObject *parent)
  : QObject(parent),
    p_sdata(new System_Data),
    p_gdata(new Graphs_Data),
    p_tdata(new Table_Data),
    m_workers(QThread::idealThreadCount()),
    m_threads(QThread::idealThreadCount())
{
  p_synchronizer = Worker_Object::getSynchronizer();
  p_synchronizer->setThreadNum(1);

  for(auto i = 0; i < m_threads.size(); ++i)
    {
      m_threads[i] = new QThread(this);
      m_workers[i] = new Worker_Object(p_sdata, p_gdata, i);
      m_workers[i]->moveToThread(m_threads[i]);
      connect(m_threads[i], &QThread::started, m_workers[i], &Worker_Object::process);
      connect(m_workers[i], &Worker_Object::signal_finished, m_threads[i], &QThread::terminate);
      connect(m_workers[i], &Worker_Object::signal_done, this, &Backend_Object::slot_generate);
    }

  p_tthread = new QThread(this);
  p_tworker = new Worker_Table(p_synchronizer, p_sdata, p_tdata);
  p_tworker->moveToThread(p_tthread);

  connect(p_tthread, &QThread::started,              p_tworker,    &Worker_Table::process);
  connect(p_tworker, &Worker_Table::signal_finished, p_tthread,    &QThread::terminate);
  connect(p_tworker, &Worker_Table::signal_finished, this,         &Backend_Object::signal_done);
}

Backend_Object::~Backend_Object( )
{
  for(auto& worker : m_workers)
    {
      delete worker;
    }
  delete p_tworker;
}

std::shared_ptr<Progress> Backend_Object::getProgress()
{
  return p_synchronizer->getProgress();
}

std::shared_ptr<System_Data> Backend_Object::system_data()
{
  return p_sdata;
}

std::shared_ptr<Graphs_Data> Backend_Object::graphs_data()
{
  return p_gdata;
}

std::shared_ptr<Table_Data> Backend_Object::table_data()
{
  return p_tdata;
}

void Backend_Object::slot_start()
{
  p_synchronizer->setThreadNum(p_sdata->thr_num);

  for(auto i = 0; i < p_synchronizer->getThreadNum(); ++i)
    {
      m_threads[i]->start();
    }
}

void Backend_Object::slot_pause()
{
  p_synchronizer->pause();
}

void Backend_Object::slot_resume()
{
  p_synchronizer->resume();
}

void Backend_Object::slot_stop()
{
  p_synchronizer->cancel();
}

void Backend_Object::slot_generate()
{
  if(!p_synchronizer->canceled())
    {
      p_gdata->update();
      p_tthread->start();
    }
  p_synchronizer->resetBarrier();
  p_synchronizer->resetCancel();
}

