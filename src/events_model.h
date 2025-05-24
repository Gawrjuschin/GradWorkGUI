#ifndef EVENTS_MODEL_H
#define EVENTS_MODEL_H

#include <QAbstractTableModel>

#include <QueSys/queueing_system.h>

#include <iosfwd>

/**
 * @brief The EventModel class  - модель событий СМО. Используется таблицей
 * событий при симуляции первых 1000 событий.
 */
class EventModel : public QAbstractTableModel {
  Q_OBJECT

  QVector<queueing_system::Event> m_data;

public:
  /**
   * @brief kColCount - количество столбцов в таблице
   */
  static constexpr auto kColCount{7};

  explicit EventModel(QObject* parent = nullptr);

  ~EventModel();

  /**
   * @brief rowCount - перегрузка метода
   * @return
   */
  int rowCount(const QModelIndex&) const override;

  /**
   * @brief columnCount - перегрузка метода
   * @return
   */
  int columnCount(const QModelIndex&) const override;

  /**
   * @brief data - перегрузка метода
   * @param index
   * @param role
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
  void swap(QVector<queueing_system::Event>& vec);
  /**
   * @brief asText - генерация строки, которую можно вставить в Word
   * @return
   */
  std::ostream& asText(std::ostream& os) const;
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

#endif // EVENTS_MODEL_H
