/*******************************************************************************
 * @copyright Copyright [2023] <Phylis Cristobal, Morris Roggo>
 * @brief A1_Maze-1.
 * @file controller.cc
 * @date 2023-10-11
 * @author phyliscr, morrisro
 ********************************************************************************/
#include "controller.h"

namespace s21 {

Controller::Controller(ModelType model_type, const std::string &filename)
    : model_type_(model_type), model_maze_(nullptr), model_cave_(nullptr) {
  if (model_type_ == ModelType::MAZE)
    model_maze_ = new ModelMaze(filename);
  else
    model_cave_ = new ModelCave(filename);
}

Controller::Controller(uint16_t row, uint16_t column)
    : model_type_(ModelType::MAZE), model_maze_(nullptr), model_cave_(nullptr) {
  model_maze_ = new ModelMaze(row, column);
}

Controller::Controller(uint16_t row, uint16_t column, uint16_t odds)
    : model_type_(ModelType::CAVE), model_maze_(nullptr), model_cave_(nullptr) {
  model_cave_ = new ModelCave(row, column, odds);
}

Controller::~Controller() {}

}  // namespace s21
