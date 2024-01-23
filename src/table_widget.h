#ifndef TABLE_WIDGET_H
#define TABLE_WIDGET_H

#include <QWidget>

class Table_Data;
class QTableView;

class Table_Widget : public QWidget
{
  Q_OBJECT

public:
  explicit Table_Widget(Table_Data* tdata, QWidget* parent = nullptr);
  ~Table_Widget();

protected:
  void paintEvent(QPaintEvent* event) override;

private:
  QTableView* p_tab_reqs;
  QTableView* p_tab_evs;
};

#endif // TABLE_WIDGET_H
