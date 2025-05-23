#ifndef GRAPHS_SWITCH_H
#define GRAPHS_SWITCH_H

#include <QWidget>
#include <array>

class QPushButton;
class QMenu;
class QComboBox;
class QGroupBox;

/**
 * @brief The GraphsSwitch class - виджет переключателя чартов.
 */
class GraphsSwitch : public QWidget {
  Q_OBJECT

  static constexpr std::size_t BUTTONS_COUNT{3};
  static constexpr std::size_t GRAPHS_COUNT{6};

  std::array<QPushButton*, BUTTONS_COUNT> m_graphs_buttons;
  std::array<QMenu*, GRAPHS_COUNT> m_graphs_menus;
  QComboBox* p_prior_cbox;
  int m_selected_graph{0};

public:
  explicit GraphsSwitch(QWidget* parent = nullptr);

  ~GraphsSwitch() = default;

  /**
   * @brief reset - сброс состояния переключателя
   */
  void reset();

protected slots:
  /**
   * @brief onApproximation - слот активации апроксимации. Срабатывает при
   * нажатии на галочку апроксимации в меню кнопки чарта.
   */
  void onApproximation();
  /**
   * @brief onSave - слот сохрнения чарта в файл. Срабатывает при нажатии кнопки
   * сохранения. Открывает стандартный диалог сохранения, после чего испускает
   * сигнал sigSave с путём до файла.
   */
  void onSave();
  /**
   * @brief onPriorityChange - слот переключения приоритета. Срабатывает при
   * переключении комбобокса приоритета.
   */
  void onPriorityChange(int);

signals:
  /**
   * @brief sigShow - сигнал переключения на указанный чарт
   */
  void sigShow(int);
  /**
   * @brief sigApproximate - сигнал включения отображения апроксимации графика
   * для указанного чарта
   */
  void sigApproximate(int);
  /**
   * @brief sigDeapproximate - сигнал отключения отображения апроксимации
   * графика для указанного чарта
   */
  void sigDeapproximate(int);
  /**
   * @brief sigSave - сигнал сохранения активного чарта
   * @param filename - имя чарта зависит от выбранного графика
   */
  void sigSave(QString filename);
  /**
   * @brief sigZoomIn - сигнал приближения чарта
   */
  void sigZoomIn();
  /**
   * @brief sigZoomOut - сигнал отдаления чарта
   */
  void sigZoomOut();
  /**
   * @brief sigZoomReset - сигнал сброса приближения чарта
   */
  void sigZoomReset();

private:
  QGroupBox* MakeGraphsGroup();
  QGroupBox* MakeZoomGroup();
  void CreateMenus();
};

#endif // GRAPHS_SWITCH_H
