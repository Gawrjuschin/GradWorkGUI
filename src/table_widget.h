#ifndef TABLE_WIDGET_H
#define TABLE_WIDGET_H

#include <QWidget>

struct TableData;
class RequestModel;
class EventModel;
class QTableView;

/**
 * @brief The TableWidget class - виджет, отвечающий за отображение таблиц
 * событий и заявок СМО.
 */
class TableWidget : public QWidget {
  Q_OBJECT

  TableData& r_tdata;
  QTableView* p_tab_reqs;
  QTableView* p_tab_evs;
  RequestModel* p_requests_model;
  EventModel* p_events_model;

public:
  explicit TableWidget(TableData& tdata, QWidget* parent = nullptr);

  ~TableWidget();

public slots:
  /**
   * @brief onDataReady - загружает новые данные в модели таблиц из TableData.
   */
  void onDataReady();
};

#endif // TABLE_WIDGET_H
