#ifndef GRAPHS_DATA_H
#define GRAPHS_DATA_H

#include <QPointF>
#include <QVector>
#include <array>

namespace QtCharts
{
class QLineSeries;
}

class Graphs_Data
{

public:
    enum
    {
        W_0 = 0,
        W_1,
        U_0,
        U_1,
        Z_0,
        Z_1,
        GRAPHS_COUNT,
        POINTS_COUNT = 1000
    };

    Graphs_Data();
    ~Graphs_Data();


    QtCharts::QLineSeries* series(int i);
    QtCharts::QLineSeries* series_apr(int i);

    QVector<QPointF>& data(int i);
    QVector<QPointF>& data_apr(int i);

    QPointF& range(int i);

    void update();
    void update_apr(int i);

    void approximate(int index);

private:
    std::array<QtCharts::QLineSeries*, GRAPHS_COUNT>  m_points_exp;
    std::array<QtCharts::QLineSeries*, GRAPHS_COUNT>  m_points_apr;

    QVector<QVector<QPointF>>          m_exp_data;
    QVector<QVector<QPointF>>          m_apr_data;
    std::array<QPointF, GRAPHS_COUNT>  m_ranges;

private:
    void construct_series();

};

#endif // GRAPHS_DATA_H
