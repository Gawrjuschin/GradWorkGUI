#include "worker_object.h"
#include "graphs_data.h"
#include "progress.h"
#include "synchronizer.h"
#include "input_data.h"

#include <QueSys/queueing_system.h>

#include <QThread>

Worker_Object::Worker_Object(Synchronizer& synchronizer,
                             const InputData& input_data,
                             std::shared_ptr<Graphs_Data> gdata,
                             int thr_id, QObject *parent)
    : QObject(parent)
    , r_synchronizer(synchronizer)
    , r_input_data(input_data)
    , p_gdata(gdata)
    , m_thr_id(thr_id)
{

}

Worker_Object::~Worker_Object( ) = default;

void Worker_Object::process()
{
    auto lambda = InputData::min_load * r_input_data.mu * r_input_data.channels;
    m_counter = m_thr_id;
    constexpr double dL = (InputData::max_load - InputData::min_load) / Graphs_Data::POINTS_COUNT;

    //Шаг нагрузки. Остальное будет считаться через него
    auto& w_0 = p_gdata->data(0);    auto& w_1 = p_gdata->data(1);
    auto& u_0 = p_gdata->data(2);    auto& u_1 = p_gdata->data(3);
    auto& z_0 = p_gdata->data(4);    auto& z_1 = p_gdata->data(5);

    auto& progress = r_synchronizer.getProgress();

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
        Queueing_system system(lambda, r_input_data.mu, r_input_data.channels, r_input_data.propability, r_input_data.events);
        system.simulate( );

        w_0[m_counter] = { lambda, system.getWait().first  };
        w_1[m_counter] = { lambda, system.getWait().second };
        u_0[m_counter] = { lambda, system.getUtility().first  };
        u_1[m_counter] = { lambda, system.getUtility().second };
        z_0[m_counter] = { lambda, system.getZ().first  };
        z_1[m_counter] = { lambda, system.getZ().second };

        m_counter += r_synchronizer.getThreadNum();
        ++r_synchronizer;
    }

    // Инициализирую минимум и максимум первой точкой
    for( auto i = 0; i < Graphs_Data::GRAPHS_COUNT; ++i )
    {
        p_gdata->range(i) = { p_gdata->data(i).front().y(),
                             p_gdata->data(i).front().y() };
    }

    while(m_counter < Graphs_Data::POINTS_COUNT && !r_synchronizer.canceled())
    {
        r_synchronizer.sleep(); // Реализация паузы
        lambda = (InputData::min_load + dL * m_counter) * (r_input_data.mu * r_input_data.channels);

        Queueing_system system(lambda, r_input_data.mu, r_input_data.channels, r_input_data.propability, r_input_data.events);
        system.simulate( );

        w_0[m_counter] = { lambda, system.getWait().first  };
        w_1[m_counter] = { lambda, system.getWait().second };
        u_0[m_counter] = { lambda, system.getUtility().first  };
        u_1[m_counter] = { lambda, system.getUtility().second };
        z_0[m_counter] = { lambda, system.getZ().first  };
        z_1[m_counter] = { lambda, system.getZ().second };

        range_calc(m_counter);

        m_counter += r_synchronizer.getThreadNum();
        ++progress;
    }

    if(++r_synchronizer >= r_synchronizer.getThreadNum())
    {
        emit signal_done();
    }
    emit signal_finished();
}
