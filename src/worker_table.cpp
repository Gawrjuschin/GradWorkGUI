#include "worker_table.h"
#include "models.h"
#include "synchronizer.h"
#include "system_data.h"
#include "table_data.h"

#include <QueSys/queueing_system.h>

#include <QThread>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

std::ostream& operator <<(std::ostream& os,  const Request& r)
{
  return os << r.first << '\t' << r.second;
}

std::istream& operator >>(std::istream& is, Request& r)
{
  return is >> std::skipws >> r.first >> r.second;
}

Worker_Table::Worker_Table( std::shared_ptr<Synchronizer> synchronizer,
                            std::shared_ptr<System_Data> system_data,
                            std::shared_ptr<Table_Data> table_data,
                            QObject *parent)
  : QObject(parent)
  , p_sync(synchronizer)
  , p_parameters(system_data)
  , p_tdata(table_data)
{

}

Worker_Table::~Worker_Table() = default;

void Worker_Table::process()
{
  auto lambda = p_parameters->load.y() * p_parameters->mu * p_parameters->ch_num;

  Queueing_system system(lambda, p_parameters->mu, p_parameters->ch_num,
                         p_parameters->prop, p_parameters->ev_num);

  std::stringstream reqs;
  std::stringstream evs;

  system.simulate(evs, reqs);

  auto reqs_generated = system.getReqNum();
  auto reqs_in_queue = system.q_status().first + system.q_status().second;
  parce_reqs(reqs, reqs_generated, reqs_in_queue);
  parce_evs(evs, system.getMaxEvNum());

  if(!p_sync->canceled())
    {
      emit signal_finished();
    }
}


void Worker_Table::parce_reqs(std::istream& reqs, int s_reqs, int q_reqs)
{
  auto* r_model = p_tdata->request_model();

  QVector<Request> req_list;

  Request rtemp;

  reqs.ignore(std::numeric_limits<std::streamsize>::max(),reqs.widen('\n'));
  reqs.ignore(std::numeric_limits<std::streamsize>::max(),reqs.widen('\n'));

  for(auto i = 0; i < s_reqs - q_reqs; ++i)
    {
      reqs >> rtemp;
      req_list << rtemp;
    }

  reqs.ignore(std::numeric_limits<std::streamsize>::max(),reqs.widen('\n'));
  reqs.ignore(std::numeric_limits<std::streamsize>::max(),reqs.widen('\n'));
  reqs.ignore(std::numeric_limits<std::streamsize>::max(),reqs.widen('\n'));

  for(auto i = 0; i < q_reqs; ++i)
    {
      reqs >> rtemp;
      req_list << rtemp;
    }

  r_model->clear();
  r_model->append(req_list);
}

void Worker_Table::parce_evs(std::istream& evs, int evs_count)
{
  auto* e_model = p_tdata->event_model();

  QVector<Event>   ev_list;

  Event etemp;

  evs.ignore(std::numeric_limits<std::streamsize>::max(),evs.widen('\n'));
  evs.ignore(std::numeric_limits<std::streamsize>::max(),evs.widen('\n'));

  for(auto i = 0; i < evs_count; ++i)
    {
      evs >> etemp;
      ev_list << etemp;
    }
  e_model->clear();
  e_model->append(ev_list);
}
