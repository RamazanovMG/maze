/*******************************************************************************
 * @copyright Copyright [2023] <Phylis Cristobal, Morris Roggo>
 * @brief A1_Maze-1.
 * @file scene.cc
 * @date 2023-10-16
 * @author phyliscr, morrisro
 ********************************************************************************/
#include "scene.h"

namespace s21 {

CustomGraphicsScene::CustomGraphicsScene(QObject *parent)
    : QGraphicsScene(parent) {}

void CustomGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  // Обработка события клика мыши
  QPointF clickPos = event->scenePos();
  emit mouseClick(clickPos);
  // Передаем управление родительскому классу
  QGraphicsScene::mousePressEvent(event);
}

} // namespace s21
