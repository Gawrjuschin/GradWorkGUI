#ifndef SYSTEM_DATA_H
#define SYSTEM_DATA_H

#include <QPointF>

struct System_Data
{
  System_Data() = default;

  void set(QPointF _load, double _mu, int _ch_num, double _prop,
           int _thr_num, int _ev_num)
  {
    load = _load;
    mu = _mu;
    ch_num = _ch_num;
    prop = _prop;
    thr_num = _thr_num;
    ev_num = _ev_num;
  }

  QPointF load{0,0};
  double mu{0};
  int ch_num{0};
  double prop{0};
  int thr_num{0};
  int ev_num{0};
};

#endif // SYSTEM_DATA_H
