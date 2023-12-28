#include "backend_object.h"
#include "graphs_data.h"
#include "input_data.h"
#include "models.h"
#include "synchronizer.h"
#include "table_data.h"
#include "worker_object.h"
#include "worker_table.h"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QTabWidget>
#include <QTableView>
#include <QThread>

Backend_Object::Backend_Object(const InputData& input_data, QObject *parent)
  : QObject(parent),
    r_input_data(input_data),
    p_gdata(new Graphs_Data),
    p_tdata(new Table_Data),
    m_workers(QThread::idealThreadCount()),
    m_threads(QThread::idealThreadCount())
{
  m_synchronizer.setThreadNum(1);

  for(auto i = 0; i < m_threads.size(); ++i)
    {
      m_threads[i] = new QThread(this);
      m_workers[i] = new Worker_Object(m_synchronizer, input_data, p_gdata, i);
      m_workers[i]->moveToThread(m_threads[i]);
      connect(m_threads[i], &QThread::started, m_workers[i], &Worker_Object::process);
      connect(m_workers[i], &Worker_Object::signal_finished, m_threads[i], &QThread::terminate);
      connect(m_workers[i], &Worker_Object::signal_done, this, &Backend_Object::slot_generate);
    }

  p_tthread = new QThread(this);
  p_tworker = new Worker_Table(m_synchronizer, input_data, p_tdata);
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

const Progress& Backend_Object::getProgress() const noexcept
{
  return m_synchronizer.getProgress();
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
  m_synchronizer.setThreadNum(r_input_data.threads);

  for(auto i = 0; i < m_synchronizer.getThreadNum(); ++i)
    {
      m_threads[i]->start();
    }
}

void Backend_Object::slot_pause()
{
  m_synchronizer.pause();
}

void Backend_Object::slot_resume()
{
  m_synchronizer.resume();
}

void Backend_Object::slot_stop()
{
  m_synchronizer.cancel();
  m_synchronizer.getProgress().resetProgress();
}

void Backend_Object::slot_generate()
{
  if(!m_synchronizer.canceled())
    {
      p_gdata->update();
      p_tthread->start();
    }
  m_synchronizer.resetBarrier();
  m_synchronizer.resetCancel();
}

