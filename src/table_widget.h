#ifndef TABLE_WIDGET_H
#define TABLE_WIDGET_H

#include <QWidget>

struct TableData;
class RequestModel;
class EventModel;
class QTableView;

class TableWidget : public QWidget {
  Q_OBJECT

public:
  explicit TableWidget(TableData& tdata, QWidget* parent = nullptr);
  ~TableWidget();

public slots:
  void onDataReady();

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
