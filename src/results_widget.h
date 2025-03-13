#ifndef RESULTS_WIDGET_H
#define RESULTS_WIDGET_H

#include <QWidget>

struct TableData;
class PointsData;
class QStackedWidget;
class QTabWidget;
class TableWidget;
class Graphs_Widget;
class Loading_Widget;

class Results_Widget : public QWidget
{
  Q_OBJECT

public:
  Results_Widget(TableData& tdata, const PointsData& gdata,
                 QWidget* parent = nullptr);
  ~Results_Widget();

protected:
  void paintEvent(QPaintEvent* event) override;

public slots:
  void onStart();
  void onPause();
  void onResume();
  void onStop();
  void onDataReady();

signals:
  void sigDataReady();

private:
  QStackedWidget* p_widget_host;
  QTabWidget* p_tabs_widget;
  TableWidget* p_table_widget;
  Graphs_Widget* p_graphs_widget;
  Loading_Widget* p_loading_widget;

  enum { WAITING = 0, PROCESSING = 1, PAUSED = 2, READY = 3 };
};

#endif // RESULTS_WIDGET_H
