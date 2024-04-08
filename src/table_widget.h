#ifndef TABLE_WIDGET_H
#define TABLE_WIDGET_H

#include <QWidget>

class TableData;
class RequestModel;
class EventModel;
class QTableView;

class Table_Widget : public QWidget
{
  Q_OBJECT

public:
  explicit Table_Widget(TableData& tdata, QWidget* parent = nullptr);
  ~Table_Widget();

public slots:
  void onReady();

protected:
  // void paintEvent(QPaintEvent* event) override;

private:
  TableData& r_tdata;
  QTableView* p_tab_reqs;
  QTableView* p_tab_evs;
  RequestModel* p_model_reqs;
  EventModel* p_model_evs;
};

#endif // TABLE_WIDGET_H
