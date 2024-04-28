/*******************************************************************************
 * @copyright Copyright [2023] <Phylis Cristobal, Morris Roggo>
 * @brief A1_Maze-1.
 * @file mainwindow.cc
 * @date 2023-10-16
 * @author phyliscr, morrisro
 ********************************************************************************/
#include "mainwindow.h"

#include "./ui_mainwindow.h"

#define SIZE 500

namespace s21 {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow), controller_(nullptr),
      model_type_(Controller::ModelType::MAZE), k_column_(0), k_row_(0),
      clear_counter_flag_(false) {
  ui_->setupUi(this);
  scene_ = new CustomGraphicsScene;
  view_ = new QGraphicsView(scene_);
  view_->setStyleSheet("border: none;");
  view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  QVBoxLayout *view_layout = new QVBoxLayout(ui_->widgetView);
  view_layout->addWidget(view_);
  ui_->widgetView->setLayout(view_layout);

  wall_pen_ = QPen(QBrush(palette().color(QPalette::Highlight)), 2);
  path_pen_ = QPen(QColor(0, 211, 202), 2);

  InitEllipsePoint();
  InitButtons();

  connect(scene_, &CustomGraphicsScene::mouseClick, this,
          &MainWindow::SceneMouseClick);

  connect(&timer_cave_, &QTimer::timeout, this,
          &MainWindow::onTimerTimeoutCave);
}

MainWindow::~MainWindow() {
  delete controller_;
  delete first_path_ellipse_;
  delete second_path_ellipse_;
  delete scene_;
  delete view_;
  delete ui_;
}

void MainWindow::InitEllipsePoint() {
  first_path_ellipse_ = new QGraphicsEllipseItem(0, 0, 3, 3);
  second_path_ellipse_ = new QGraphicsEllipseItem(0, 0, 3, 3);
  first_path_ellipse_->setPen(path_pen_);
  first_path_ellipse_->setVisible(false);
  scene_->addItem(first_path_ellipse_);
  second_path_ellipse_->setPen(path_pen_);
  second_path_ellipse_->setVisible(false);
  scene_->addItem(second_path_ellipse_);
}

void MainWindow::SetWindowName(QString name) {
  if (std::rand() % 2)
    setWindowTitle(name + " (written by PhylisCr, MorrisRo)");
  else
    setWindowTitle(name + " (written by MorrisRo, PhylisCr)");
}

void MainWindow::InitButtons() {
  if (model_type_ == Controller::ModelType::MAZE) {
    SetWindowName("MAZE");
    setFixedSize(SIZE + 40, SIZE + 136);
    SetVisibleCaveButtons(false);

  } else {
    setFixedSize(SIZE + 40, SIZE + 145);
    SetWindowName("CAVE");
    SetVisibleCaveButtons(true);
  }
}

void MainWindow::SetVisibleCaveButtons(bool state) {
  ui_->lblLive->setVisible(state);
  ui_->sbLive->setVisible(state);
  ui_->lblBorn->setVisible(state);
  ui_->sbBorn->setVisible(state);
  ui_->lblTimeout->setVisible(state);
  ui_->sbTimeout->setVisible(state);
  ui_->lblOdds->setVisible(state);
  ui_->sbOdds->setVisible(state);
  ui_->pbPlay->setVisible(state);
}

void MainWindow::SetEnableAllButtons(bool state) {
  ui_->sbRows->setEnabled(state);
  ui_->sbCols->setEnabled(state);
  ui_->sbLive->setEnabled(state);
  ui_->sbBorn->setEnabled(state);
  ui_->sbTimeout->setEnabled(state);
  ui_->sbOdds->setEnabled(state);
  ui_->pbGenerate->setEnabled(state);
  ui_->menubar->setEnabled(state);
}

void MainWindow::InitDrawRatio() {
  uint16_t rows = controller_->GetRows();
  uint16_t columns = controller_->GetColumns();
  k_column_ = SIZE / columns;
  k_row_ = SIZE / rows;
}

void MainWindow::ClearScreen() {
  first_path_ellipse_->setPos(0, 0);
  second_path_ellipse_->setPos(0, 0);
  first_path_ellipse_->setVisible(false);
  second_path_ellipse_->setVisible(false);
  delete first_path_ellipse_;
  delete second_path_ellipse_;
  scene_->clear();
  InitEllipsePoint();
}

void MainWindow::ClearController() {
  delete controller_;
  controller_ = nullptr;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                  MENU
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void MainWindow::on_actionOpen_Maze_triggered() {
  QString filename = QFileDialog::getOpenFileName(
      this, "Open Maze file", "", "Maze files (*.maze);;Все файлы (*)");
  ui_->statusbar->showMessage(filename, 10000);
  OpenMaze(filename);
}

void MainWindow::on_actionOpen_Cave_triggered() {
  QString filename = QFileDialog::getOpenFileName(
      this, "Open Maze file", "", "Cave files (*.cave);;Все файлы (*)");
  ui_->statusbar->showMessage(filename, 10000);
  OpenCave(filename);
}

void MainWindow::on_actionExit_triggered() { QCoreApplication::quit(); }

void MainWindow::on_actionMaze_triggered() { NewMaze(); }

void MainWindow::on_actionCave_triggered() { NewCave(); }

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                 BUTTONS
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void MainWindow::on_pbGenerate_clicked() {
  if (model_type_ == Controller::ModelType::MAZE)
    NewMaze(ui_->sbRows->value(), ui_->sbCols->value());
  else
    NewCave(ui_->sbRows->value(), ui_->sbCols->value(), ui_->sbOdds->value());
}

void MainWindow::on_pbPlay_clicked() {
  if (controller_ != nullptr) {
    if (timer_cave_.isActive()) {
      ui_->pbPlay->setText("Start");
      SetEnableAllButtons(true);
      timer_cave_.stop();
    } else if (ui_->sbTimeout->value()) {
      timer_cave_.start(ui_->sbTimeout->value());
      ui_->statusbar->showMessage("Feel like a god ... Ready ?");
      ui_->pbPlay->setText("Stop");
      SetEnableAllButtons(false);
    } else {
      StepCave();
    }
  }
}

void MainWindow::onTimerTimeoutCave() {
  static size_t counter;

  if (clear_counter_flag_) {
    clear_counter_flag_ = false;
    counter = 0;
  } else {
    StepCave();
    ++counter;
    ui_->statusbar->showMessage("Frames counter: " + QString::number(counter),
                                10000);
  }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                  MOUSE
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void MainWindow::SceneMouseClick(const QPointF &pos) {
  if (controller_ != nullptr) {
    static QString tmp_string;
    if (first_path_ellipse_->isVisible() == false) {
      tmp_string = CheckPathPoints(first_path_ellipse_, pos);
      path_first_ = std::pair(first_path_ellipse_->pos().y() / k_row_,
                              first_path_ellipse_->pos().x() / k_column_);

    } else if (second_path_ellipse_->isVisible() == false) {
      tmp_string += CheckPathPoints(second_path_ellipse_, pos);
      path_second_ = std::pair(second_path_ellipse_->pos().y() / k_row_,
                               second_path_ellipse_->pos().x() / k_column_);
      DrawMazePath(controller_->find_path(path_first_, path_second_));
    } else {
      first_path_ellipse_->setVisible(false);
      second_path_ellipse_->setVisible(false);
      ClearScreen();
      DrawMaze();
    }
    ui_->statusbar->showMessage(tmp_string, 10000);
  }
}

QString MainWindow::CheckPathPoints(QGraphicsEllipseItem *point,
                                    const QPointF &pos) {
  uint16_t point_path_row;
  uint16_t point_path_column;
  uint16_t k_optic_center_ = 2;
  QString tmp_string;
  point_path_row = static_cast<int>(pos.x()) / k_column_;
  point_path_column = static_cast<int>(pos.y()) / k_row_;

  point->setPos(k_column_ / 2 - k_optic_center_ + (k_column_ * point_path_row),
                k_row_ / 2 - k_optic_center_ + (k_row_ * point_path_column));
  point->setVisible(true);
  tmp_string = "[" + QString::number(point_path_row) + "," +
               QString::number(point_path_column) + "] ";

  return tmp_string;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                  MAZE
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/*!
 * \brief Создание новой модели и удаление старой.
 * \param rows Количество строк.
 * \param columns Количество столбцов.
 */
void MainWindow::NewMaze() {
  model_type_ = Controller::ModelType::MAZE;
  InitButtons();
  ClearScreen();
  ClearController();
}

/*!
 * \brief Генерирование новой модели.
 * \param rows Количество строк.
 * \param columns Количество столбцов.
 */
void MainWindow::NewMaze(uint16_t rows, uint16_t columns) {
  NewMaze();
  controller_ = new Controller(rows, columns);
  DrawMaze();
}

/*!
 * \brief MainWindow::OpenMaze Открытие существующей модели из файла.
 * \param filename Абсолютный путь до файла.
 */
void MainWindow::OpenMaze(const QString &filename) {
  NewMaze();
  try {
    controller_ = new Controller(model_type_, filename.toStdString());
    DrawMaze();
  } catch (const std::exception &e) {
    QMessageBox::critical(nullptr, "Ошибка при открытии файла.", e.what());
  }
}

/*!
 * \brief MainWindow::DrawMaze Рисование лабиринта на сцене.
 */
void MainWindow::DrawMaze() {
  InitDrawRatio();

  for (uint16_t row = 0; row < controller_->GetRows(); ++row) {
    for (uint16_t col = 0; col < controller_->GetColumns(); ++col) {
      if (row == 0)
        scene_->addLine(k_column_ * ((col + 1) - 1), 0, k_column_ * (col + 1),
                        0, wall_pen_);
      if (col == 0)
        scene_->addLine(0, k_row_ * ((row + 1) - 1), 0, k_row_ * (row + 1),
                        wall_pen_);
      if (controller_->GetValueRight(row, col))
        scene_->addLine(k_column_ * (col + 1), k_row_ * ((row + 1) - 1),
                        k_column_ * (col + 1), k_row_ * (row + 1), wall_pen_);
      if (controller_->GetValueBottom(row, col))
        scene_->addLine(k_column_ * ((col + 1) - 1), k_row_ * (row + 1),
                        k_column_ * (col + 1), k_row_ * (row + 1), wall_pen_);
    }
  }
}

/*!
 * \brief MainWindow::DrawMazePath Рисование пути на сцене.
 */
void MainWindow::DrawMazePath(
    const std::vector<std::pair<uint16_t, uint16_t>> &coordinates) {
  for (size_t i = 1; i < coordinates.size(); ++i) {
    int x1 = k_column_ / 2 + (k_column_ * (coordinates[i - 1].second));
    int y1 = k_row_ / 2 + (k_row_ * (coordinates[i - 1].first));
    int x2 = k_column_ / 2 + (k_column_ * (coordinates[i].second));
    int y2 = k_row_ / 2 + (k_row_ * (coordinates[i].first));
    QGraphicsLineItem *path = new QGraphicsLineItem(x1, y1, x2, y2);
    path->setPen(path_pen_);
    scene_->addItem(path);
  }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                  CAVE
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
/*!
 * \brief Создание новой модели и удаление старой.
 * \param rows Количество строк.
 * \param columns Количество столбцов.
 */
void MainWindow::NewCave() {
  model_type_ = Controller::ModelType::CAVE;
  InitButtons();
  ClearScreen();
  ClearController();
  clear_counter_flag_ = true;
}

/*!
 * \brief Генерирование новой модели пещеры.
 * \param rows Количество строк.
 * \param columns Количество столбцов.
 */
void MainWindow::NewCave(uint16_t rows, uint16_t columns, uint16_t odds) {
  NewCave();
  controller_ = new Controller(rows, columns, odds);
  DrawCave();
}

/*!
 * \brief MainWindow::OpenCave Открытие существующей модели из файла.
 * \param filename Абсолютный путь до файла.
 */
void MainWindow::OpenCave(const QString &filename) {
  NewCave();
  try {
    controller_ =
        new Controller(Controller::ModelType::CAVE, filename.toStdString());
    DrawCave();
  } catch (const std::exception &e) {
    QMessageBox::critical(nullptr, "Ошибка при открытии файла.", e.what());
  }
}

/*!
 * \brief MainWindow::StepCave Один шаг генератора пещеры.
 */
bool MainWindow::StepCave() {
  bool return_flag = false;
  if (controller_ == nullptr) {
    QMessageBox::warning(nullptr, "Ошибка",
                         "Необходимо открыть файл, в котором описана пещера!");
  } else {
    return_flag =
        controller_->OneStep(ui_->sbLive->value(), ui_->sbBorn->value());
    ClearScreen();
    DrawCave();
  }
  return return_flag;
}

/*!
 * \brief MainWindow::DrawCave Рисование пещеры на сцене.
 */
void MainWindow::DrawCave() {
  InitDrawRatio();
  for (uint16_t row = 0; row < controller_->GetRows(); ++row) {
    for (uint16_t col = 0; col < controller_->GetColumns(); ++col) {
      if (controller_->GetValue(row, col))
        scene_->addRect(k_column_ * col, k_row_ * row, k_column_, k_row_,
                        QPen(QBrush(palette().color(QPalette::Highlight)), 0),
                        QBrush(palette().color(QPalette::Highlight)));
    }
  }
}

} // namespace s21
