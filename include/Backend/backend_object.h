#ifndef BACKEND_OBJECT_H
#define BACKEND_OBJECT_H

#include <QObject>
#include <memory>

struct System_Data;
class Graphs_Data;
class Table_Data;
class Worker_Object;
class Worker_Table;
class Progress;
class Synchronizer;

class Backend_Object : public QObject
{
    Q_OBJECT

public:
  explicit Backend_Object(QObject *parent = nullptr);
  ~Backend_Object();

  std::shared_ptr<System_Data> system_data();
  std::shared_ptr<Graphs_Data> graphs_data();
  std::shared_ptr<Table_Data>  table_data();
  std::shared_ptr<Progress>    getProgress();

public slots:
  void slot_start();
  void slot_pause();
  void slot_resume();
  void slot_stop();
  void slot_generate();

signals:
  void signal_start();
  void signal_table_fill();
  void signal_done();
  void signal_progress();

private:
  //Data:
  std::shared_ptr<System_Data>       p_sdata;
  std::shared_ptr<Graphs_Data>       p_gdata;
  std::shared_ptr<Table_Data>        p_tdata;
  //Threads:
  QVector<Worker_Object*>       m_workers;
  QVector<QThread*>             m_threads;
  Worker_Table*                 p_tworker;
  QThread*                      p_tthread;
  std::shared_ptr<Synchronizer> p_synchronizer;

};

#endif // BACKEND_OBJECT_H
