#ifndef BACKEND_OBJECT_H
#define BACKEND_OBJECT_H

// #include "synchronizer.h"

// #include <QObject>
// #include <memory>

// class QThreadPool;

// class Graphs_Data;
// class Table_Data;
// class Worker_Object;
// class Worker_Table;
// class Progress;

// struct InputData;

// struct BackendDataImpl;

// class Backend_Object : public QObject
// {
//   Q_OBJECT

// public:
//   explicit Backend_Object(const InputData &input_data, QObject *parent = nullptr);
//   ~Backend_Object() override;

//   const Graphs_Data &graphs_data();
//   const Table_Data &table_data();
//   const Progress &getProgress() const noexcept;

// public slots:
//   void slot_start();
//   void slot_pause();
//   void slot_resume();
//   void slot_stop();

// signals:
//   void signal_done();

// private:
//   const InputData &r_input_data;
//   // Data:
//   std::unique_ptr<BackendDataImpl> p_data;
//   QThreadPool *p_threadPool;
//   Synchronizer m_synchronizer{};
// };

#endif // BACKEND_OBJECT_H
