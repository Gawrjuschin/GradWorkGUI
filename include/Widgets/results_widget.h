#ifndef RESULTS_WIDGET_H
#define RESULTS_WIDGET_H

#include <QWidget>
#include <memory>

class Table_Data;
class Graphs_Data;
class QStackedWidget;
class QTabWidget;
class Table_Widget;
class Graphs_Widget;
class Loading_Widget;

class Results_Widget : public QWidget
{
  Q_OBJECT

public:
  Results_Widget(std::shared_ptr<Table_Data> tdata,
                 std::shared_ptr<Graphs_Data> gdata,
                 QWidget *parent = nullptr);
  ~Results_Widget( );

protected:
  void paintEvent(QPaintEvent* event) override;

public slots:
  void slot_start();
  void slot_pause();
  void slot_resume();
  void slot_stop();
  void slot_done();

signals:
  void signal_prepare();
  void signal_ready();

private:
  QStackedWidget* p_widget_host;
  QTabWidget*     p_tabs_tables_graphs;
  Table_Widget*   p_tables_evs_reqs;
  Graphs_Widget*  p_widget_graphs;
  Loading_Widget* p_widget_loading;

  enum
  {
    WAITING      = 0,
    PROCESSING   = 1,
    PAUSED       = 2,
    READY        = 3
  };
};

#endif // RESULTS_WIDGET_H
