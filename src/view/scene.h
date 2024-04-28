/*******************************************************************************
 * @copyright Copyright [2023] <Phylis Cristobal, Morris Roggo>
 * @brief A1_Maze-1.
 * @file scene.h
 * @date 2023-10-16
 * @author phyliscr, morrisro
 ********************************************************************************/
#ifndef A1_MAZE_1_VIEW_SCENE_H_
#define A1_MAZE_1_VIEW_SCENE_H_

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>

namespace s21 {

class CustomGraphicsScene : public QGraphicsScene {
  Q_OBJECT

public:
  CustomGraphicsScene(QObject *parent = nullptr);

signals:
  void mouseClick(const QPointF &pos);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

} // namespace s21

#endif // A1_MAZE_1_VIEW_SCENE_H_
