#ifndef BACKEND_OBJECT_H
#define BACKEND_OBJECT_H

#include "synchronizer.h"

#include <QObject>
#include <array>
#include <memory>

class Graphs_Data;
class Table_Data;
class Worker_Object;
class Worker_Table;
class Progress;

struct InputData;
struct TableData;

class Backend_Object : public QObject
{
  Q_OBJECT

public:
  explicit Backend_Object(const InputData &input_data, QObject *parent = nullptr);
  ~Backend_Object();

  std::shared_ptr<Graphs_Data> graphs_data();
  std::shared_ptr<Table_Data> table_data();
  const Progress &getProgress() const noexcept;

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
  const InputData &r_input_data;
  // Data:
  std::shared_ptr<Graphs_Data> p_gdata;
  std::shared_ptr<Table_Data> p_tdata;
  // Threads:
  QVector<Worker_Object *> m_workers;
  QVector<QThread *> m_threads;
  Worker_Table *p_tworker;
  QThread *p_tthread;
  Synchronizer m_synchronizer{};
};

#endif // BACKEND_OBJECT_H
