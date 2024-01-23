#ifndef WORKER_TABLE_H
#define WORKER_TABLE_H

#include <QObject>
#include <memory>

class Synchronizer;
class Table_Data;
struct InputData;

class Worker_Table : public QObject
{
  Q_OBJECT

public:
  explicit Worker_Table(const Synchronizer& synchronizer,
			const InputData& input_data,
			std::shared_ptr<Table_Data> table_data,
			QObject* parent = nullptr);
  ~Worker_Table();

public slots:
  void process();

signals:
  void signal_finished();

private:
  const Synchronizer& r_synchronizer;
  const InputData& r_input_data;
  std::shared_ptr<Table_Data> p_tdata;
};

#endif // WORKER_TABLE_H
