#ifndef REQUESTS_MODEL_H
#define REQUESTS_MODEL_H

#include <QAbstractTableModel>
#include <QueSys/queueing_system.h>

/**
 * @brief The RequestModel class - модель заявок СМО. Используется таблицей
 * заявок при симуляции первых 1000 событий.
 */
class RequestModel : public QAbstractTableModel {
  Q_OBJECT

  QVector<queueing_system::Request> m_data{};

public:
  /**
   * @brief COL_COUNT - количество столбцов в таблице
   */
  static constexpr auto COL_COUNT{9};

  explicit RequestModel(QObject* parent = 0);

  ~RequestModel();

  /**
   * @brief rowCount - перегрузка метода
   * @param index
   * @return
   */
  int rowCount(const QModelIndex& index) const override;

  /**
   * @brief columnCount - перегрузка метода
   * @param index
   * @return
   */
  int columnCount(const QModelIndex& index) const override;

  /**
   * @brief data - перегрузка метода
   * @param index
   * @param role - только DisplayRole и EditRole
   * @return
   */
  QVariant data(const QModelIndex& index, int role) const override;

  /**
   * @brief headerData - перегрузка метода
   * @param section
   * @param orientation
   * @param role - только DisplayRole и EditRole
   * @return
   */
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role) const override;

  /**
   * @brief swap - обменивается векторами точек с vec
   * @param vec
   */
  void swap(QVector<queueing_system::Request>& vec);

  /**
   * @brief clear - очистка модели
   */
  void clear();

signals:
  /**
   * @brief sigUpdate - данные таблицы обновились
   */
  void sigUpdate();
};

#endif // REQUESTS_MODEL_H
