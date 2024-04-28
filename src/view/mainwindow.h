/*******************************************************************************
 * @copyright Copyright [2023] <Phylis Cristobal, Morris Roggo>
 * @brief A1_Maze-1.
 * @file mainwindow.h
 * @date 2023-10-16
 * @author phyliscr, morrisro
 ********************************************************************************/
#ifndef A1_MAZE_1_VIEW_MAINWINDOW_H_
#define A1_MAZE_1_VIEW_MAINWINDOW_H_

#include <QFileDialog>
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include <QMainWindow>
#include <QMessageBox>
#include <QPointF>
#include <QProgressBar>
#include <QTimer>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "../controller/controller.h"
#include "scene.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

public slots:
  void SceneMouseClick(const QPointF &pos);
  void onTimerTimeoutCave();

private slots:
  void on_actionExit_triggered();
  void on_actionOpen_Maze_triggered();
  void on_pbGenerate_clicked();
  void on_actionOpen_Cave_triggered();
  void on_actionMaze_triggered();
  void on_actionCave_triggered();
  void on_pbPlay_clicked();

private:
  Controller *controller_;
  Controller::ModelType model_type_;
  Ui::MainWindow *ui_;
  QGraphicsView *view_;
  CustomGraphicsScene *scene_;
  QPen wall_pen_;
  QPen path_pen_;
  QGraphicsEllipseItem *first_path_ellipse_;
  QGraphicsEllipseItem *second_path_ellipse_;
  QTimer timer_cave_;
  uint16_t k_column_;
  uint16_t k_row_;
  std::pair<uint16_t, uint16_t> path_first_;
  std::pair<uint16_t, uint16_t> path_second_;
  bool clear_counter_flag_;

  void SetWindowName(QString name);
  void InitEllipsePoint();
  void InitButtons();
  void SetVisibleCaveButtons(bool state);
  void SetEnableAllButtons(bool state);
  void InitDrawRatio();
  void ClearController();
  void ClearScreen();
  QString CheckPathPoints(QGraphicsEllipseItem *point, const QPointF &pos);
  void NewMaze();
  void NewMaze(uint16_t rows, uint16_t columns);
  void OpenMaze(const QString &filename);
  void DrawMaze();
  void
  DrawMazePath(const std::vector<std::pair<uint16_t, uint16_t>> &coordinates);
  void NewCave();
  void NewCave(uint16_t rows, uint16_t columns, uint16_t odds);
  void OpenCave(const QString &filename);
  bool StepCave();
  void DrawCave();
};

} // namespace s21

#endif // A1_MAZE_1_VIEW_MAINWINDOW_H_
