#include "graphs_data.h"

#include <QLineSeries>

#include <QVector>

#include <array>
#include <cmath>



Graphs_Data::Graphs_Data()
  : m_exp_data(QVector<QVector<QPointF>>(GRAPHS_COUNT))
  , m_apr_data(QVector<QVector<QPointF>>(GRAPHS_COUNT))
{
  construct_series();
}

Graphs_Data::~Graphs_Data() = default;

void Graphs_Data::construct_series()
{
  static QPen pen_exp(Qt::blue);
  static QPen pen_apr(Qt::darkGray);
  pen_exp.setWidth(2);
  pen_apr.setWidth(2);
  pen_apr.setStyle(Qt::PenStyle::DotLine);
  for (std::size_t i = 0; i < m_points_exp.size(); ++i) {
      m_points_exp[i] = new QtCharts::QLineSeries;
      m_points_apr[i] = new QtCharts::QLineSeries;
      series(i)->setPen(pen_exp);
      series_apr(i)->setPen(pen_apr);
      m_exp_data[i].resize(POINTS_COUNT);
      m_apr_data[i].resize(POINTS_COUNT);
  }
}

QtCharts::QLineSeries* Graphs_Data::series(int i)
{
  return m_points_exp[i];
}

QtCharts::QLineSeries* Graphs_Data::series_apr(int i)
{
  return m_points_apr[i];
}

QVector<QPointF>& Graphs_Data::data(int i)
{
  return m_exp_data[i];
}

QVector<QPointF>& Graphs_Data::data_apr(int i)
{
  return m_apr_data[i];
}

QPointF& Graphs_Data::range(int i)
{
  return m_ranges[i];
}

void Graphs_Data::update()
{
  for(auto i = 0; i < m_exp_data.size(); ++i)
    {
      m_points_exp[i]->replace(m_exp_data[i]);
    }
}

void Graphs_Data::update_apr(int i)
{
  m_points_apr[i]->replace(m_apr_data[i]);
}

void Graphs_Data::approximate(int index)
{ // Применяется МНК к указанному графику
  constexpr int N = Graphs_Data::POINTS_COUNT;
  double sx = 0;
  double sx2 = 0;
  double sy = 0;
  double sxy = 0;
  auto& points = data(index);
  auto& apr    = data_apr(index);
  for(auto& pnt : points)
    {
      sx += pnt.x();
      sx2 += pnt.x() * pnt.x();
      sy += log(pnt.y());
      sxy += pnt.x() * log(pnt.y());
    }
  auto b = (N * sxy - sx * sy) / (N * sx2 - sx * sx);
  auto a = exp((sy - b * sx) / N);
  for(auto i = 0; i < N; ++i)
    {
      apr[i] = QPointF{ points[i].x(), a*exp(b*points[i].x()) };
    }
  update_apr(index);
}
