#ifndef WORKER_TABLE_H
#define WORKER_TABLE_H

#include <QObject>
#include <iosfwd>

class Synchronizer;
class Table_Data;
struct System_Data;

class Worker_Table : public QObject
{
  Q_OBJECT

public:
  explicit Worker_Table(std::shared_ptr<Synchronizer> synchronizer,
                        std::shared_ptr<System_Data> system_data,
                        std::shared_ptr<Table_Data> table_data,
                        QObject *parent = nullptr);
  ~Worker_Table();

public slots:
  void process();

signals:
  void signal_finished();

private:
  std::shared_ptr<Synchronizer>      p_sync;
  std::shared_ptr<System_Data>       p_parameters;
  std::shared_ptr<Table_Data>        p_tdata;

private:
  void parce_reqs(std::istream& reqs, int s_reqs, int q_reqs);
  void parce_evs(std::istream& evs, int evs_count);
};

#endif // WORKER_TABLE_H
