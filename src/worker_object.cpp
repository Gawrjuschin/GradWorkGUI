#include "worker_object.h"
#include "graphs_data.h"
#include "progress.h"
#include "synchronizer.h"
#include "system_data.h"

#include <QueSys/queueing_system.h>

#include <QThread>

std::shared_ptr<Synchronizer> Worker_Object::p_sync{};

Worker_Object::Worker_Object(std::shared_ptr<System_Data> sdata,
                             std::shared_ptr<Graphs_Data> gdata,
                             int thr_id, QObject *parent)
  : QObject(parent)
  , p_sdata(sdata)
  , p_gdata(gdata)
  , m_thr_id(thr_id)
{

}

Worker_Object::~Worker_Object( ) = default;

std::shared_ptr<Synchronizer> Worker_Object::getSynchronizer()
{
  if( Worker_Object::p_sync == nullptr)
    {
      Worker_Object::p_sync.reset(new Synchronizer);
    }
  return Worker_Object::p_sync;
}

void Worker_Object::process()
{
  auto lambda = p_sdata->load.x() * p_sdata->mu * p_sdata->ch_num;
  m_counter = m_thr_id;
  const double dL = (p_sdata->load.y() - p_sdata->load.x()) / Graphs_Data::POINTS_COUNT;

  //Шаг нагрузки. Остальное будет считаться через него
  auto& w_0 = p_gdata->data(0);    auto& w_1 = p_gdata->data(1);
  auto& u_0 = p_gdata->data(2);    auto& u_1 = p_gdata->data(3);
  auto& z_0 = p_gdata->data(4);    auto& z_1 = p_gdata->data(5);

  auto& progress = *p_sync->getProgress();
  auto& sync     = *p_sync;

  // Минимумы и максимумы
  auto range_calc = [ this ] (int counter)
  {
    for( auto i = 0; i < Graphs_Data::GRAPHS_COUNT; ++i )
      {
        auto point = p_gdata->data(i)[counter].y();
        QPointF& range = p_gdata->range(i);
        if(point < range.x())
          {
            range.rx() = point;
            continue;
          }
        if(point > range.y())
          {
            range.ry() = point;
            continue;
          }
      }
  };

  // Прогоняю первый раз, чтобы заполнить пределы графов
  {
    Queueing_system system(lambda, p_sdata->mu, p_sdata->ch_num, p_sdata->prop, p_sdata->ev_num);
    system.simulate( );

    w_0[m_counter] = { lambda, system.getWait().first  };
    w_1[m_counter] = { lambda, system.getWait().second };
    u_0[m_counter] = { lambda, system.getUtility().first  };
    u_1[m_counter] = { lambda, system.getUtility().second };
    z_0[m_counter] = { lambda, system.getZ().first  };
    z_1[m_counter] = { lambda, system.getZ().second };

    m_counter += sync.getThreadNum();
    ++progress;
  }

  // Инициализирую минимум и максимум первой точкой
  for( auto i = 0; i < Graphs_Data::GRAPHS_COUNT; ++i )
    {
      p_gdata->range(i) = { p_gdata->data(i).front().y(),
                            p_gdata->data(i).front().y() };
    }

  while(m_counter < Graphs_Data::POINTS_COUNT && !sync.canceled())
    {
      sync.sleep(); // Реализация паузы
      lambda = (p_sdata->load.x() + dL * m_counter) * (p_sdata->mu * p_sdata->ch_num);

      Queueing_system system(lambda, p_sdata->mu, p_sdata->ch_num, p_sdata->prop, p_sdata->ev_num);
      system.simulate( );

      w_0[m_counter] = { lambda, system.getWait().first  };
      w_1[m_counter] = { lambda, system.getWait().second };
      u_0[m_counter] = { lambda, system.getUtility().first  };
      u_1[m_counter] = { lambda, system.getUtility().second };
      z_0[m_counter] = { lambda, system.getZ().first  };
      z_1[m_counter] = { lambda, system.getZ().second };

      range_calc(m_counter);

      m_counter += sync.getThreadNum();
      ++progress;
    }

  if(++sync >= sync.getThreadNum())
    {
      emit signal_done();
    }
  emit signal_finished();
}
