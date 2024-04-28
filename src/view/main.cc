/*******************************************************************************
 * @copyright Copyright [2023] <Phylis Cristobal, Morris Roggo>
 * @brief A1_Maze-1.
 * @file main.cc
 * @date 2023-10-16
 * @author phyliscr, morrisro
 ********************************************************************************/
#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>
#include <cstdlib>
#include <ctime>

int main(int argc, char *argv[]) {
  unsigned int seed = static_cast<unsigned int>(std::time(nullptr));
  std::srand(seed);

  QApplication a(argc, argv);

  // Устанавливаем тему
  a.setStyle(QStyleFactory::create("Fusion"));

  // Устанавливаем палитру для темного режима
  QPalette darkPalette;
  darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
  darkPalette.setColor(QPalette::WindowText, Qt::white);
  darkPalette.setColor(QPalette::Base, QColor(53, 53, 53)); // (45, 45, 45)
  darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
  darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
  darkPalette.setColor(QPalette::ToolTipText, Qt::white);
  darkPalette.setColor(QPalette::Text, Qt::white);
  darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
  darkPalette.setColor(QPalette::ButtonText, Qt::white);
  darkPalette.setColor(QPalette::BrightText, Qt::red);
  darkPalette.setColor(QPalette::Link, QColor(238, 32, 77));
  darkPalette.setColor(QPalette::Highlight, QColor(238, 32, 77));
  darkPalette.setColor(QPalette::HighlightedText, Qt::black);

  a.setPalette(darkPalette);

  s21::MainWindow w;

  w.setWindowIcon(QIcon(":/icons/icon_maze.png"));
  w.show();
  return a.exec();
}
