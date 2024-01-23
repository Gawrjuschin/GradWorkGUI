#ifndef WORKER_OBJECT_H
#define WORKER_OBJECT_H

#include <QObject>
#include <memory>

class Synchronizer;
struct InputData;
class Graphs_Data;

class Worker_Object : public QObject
{
  Q_OBJECT

public:
  explicit Worker_Object(Synchronizer& synchronizer,
			 const InputData& input_data,
			 std::shared_ptr<Graphs_Data> gdata,
			 int id,
			 QObject* parent = nullptr);
  ~Worker_Object();

public slots:
  void process();

signals:
  void signal_finished();
  void signal_done();

private:
  Synchronizer& r_synchronizer;
  const InputData& r_input_data;
  std::shared_ptr<Graphs_Data> p_gdata;
  int m_thr_id;
  int m_counter{0};
};

#endif // WORKER_OBJECT_H
