#include "backend_object.h"
// #include "graphs_data.h"
// #include "input_data.h"
// // #include "synchronizer.h"
// #include "table_data.h"
// #include "threadscontrol.h"
// #include "worker_object.h"
// #include "worker_table.h"

// #include <QHBoxLayout>
// #include <QHeaderView>
// #include <QLabel>
// #include <QTabWidget>
// #include <QTableView>

// #include <QThread>
// #include <QThreadPool>

// struct BackendDataImpl
// {
//   Graphs_Data gdata{};
//   Table_Data tdata{};
// };

// Backend_Object::Backend_Object(const InputData& input_data, QObject* parent)
//     : QObject(parent)
//     , r_input_data(input_data)
//     , p_data(std::make_unique<BackendDataImpl>())
//     , p_threadPool(new QThreadPool(this))
// {}

// Backend_Object::~Backend_Object() = default;

// const Progress& Backend_Object::getProgress() const noexcept
// {
//   return m_synchronizer.getProgress();
// }

// const Graphs_Data& Backend_Object::graphs_data()
// {
//   return p_data->gdata;
// }

// const Table_Data& Backend_Object::table_data()
// {
//   return p_data->tdata;
// }

// void Backend_Object::slot_start()
// {
//   m_synchronizer.setThreadNum(r_input_data.threads);
//   p_threadPool->setMaxThreadCount(r_input_data.threads);

//   for (int thread_num = 0; thread_num < r_input_data.threads; ++thread_num) {
//     p_threadPool->start(MakeWorkPartObject(m_synchronizer,
// 					   r_input_data,
// 					   p_data->gdata,
// 					   {thread_num * Graphs_Data::POINTS_COUNT,
// 					    (thread_num + 1) * Graphs_Data::POINTS_COUNT}));
//   }

//   p_threadPool->waitForDone();

//   // TODO: расчитать пределы графов

//   if (!m_synchronizer.canceled()) {
//     p_data->gdata.update();
//     GetWorkerTableFunction(m_synchronizer, r_input_data, p_data->tdata)();
//   }
//   m_synchronizer.resetBarrier();
//   m_synchronizer.resetCancel();

//   if (!m_synchronizer.canceled()) {
//     emit signal_done();
//   }
// }

// void Backend_Object::slot_pause()
// {
//   m_synchronizer.pause();
// }

// void Backend_Object::slot_resume()
// {
//   m_synchronizer.resume();
// }

// void Backend_Object::slot_stop()
// {
//   m_synchronizer.cancel();
//   m_synchronizer.getProgress().resetProgress();
// }
