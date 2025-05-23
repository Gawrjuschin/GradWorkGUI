#ifndef RESULTS_WIDGET_H
#define RESULTS_WIDGET_H

#include <QWidget>

struct TableData;
class PointsData;
class QStackedWidget;
class QTabWidget;
class TableWidget;
class GraphsWidget;
class LoadingWidget;

/**
 * @brief The ResultsWidget class - виджет результатов симуляции.
 * До ввода первых данных отображает обычный QLabel.
 * Во время симуляции виджет переключается на LoadingWidget.
 * После завершения симуляции отобраажет две вкладки: виджет с таблицами
 * (первые 1000 событий и соответствующие заявки), виджет с графиками
 * зависимостей характеристик СМО от коэффициента нагрузки для двух приоритетов.
 * На паузе отображается обычный QLabel.
 * При остановке симуляции переключается на обычный QLable.
 */
class ResultsWidget : public QWidget {
  Q_OBJECT

  QStackedWidget* p_widget_host;
  QTabWidget* p_tabs_widget;
  TableWidget* p_table_widget;
  GraphsWidget* p_graphs_widget;
  LoadingWidget* p_loading_widget;

public:
  ResultsWidget(TableData& tdata, const PointsData& gdata,
                QWidget* parent = nullptr);

  ~ResultsWidget();

protected:
  /**
   * @brief paintEvent - перегрузка метода для отображения стилей
   * @param event
   */
  // void paintEvent(QPaintEvent* event) override;

public slots:
  /**
   * @brief onStart - при запуске симуляции переключается на LoadingWidget
   */
  void onStart();
  /**
   * @brief onPause - переключается на отображение QLabel
   */
  void onPause();
  /**
   * @brief onResume - переключается на отображение LoadingWidget
   */
  void onResume();
  /**
   * @brief onStop - переключается на отображение QLabel
   */
  void onStop();
  /**
   * @brief onDataReady - переключется на отображение вкладок с таблицами и
   * графиками
   */
  void onDataReady();

signals:
  /**
   * @brief sigDataReady - сигнал о готовности к отображению результатов
   */
  void sigDataReady();
};

#endif // RESULTS_WIDGET_H
