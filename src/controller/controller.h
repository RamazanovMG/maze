/*******************************************************************************
 * @copyright Copyright [2023] <Phylis Cristobal, Morris Roggo>
 * @brief A1_Maze-1.
 * @file controller.h
 * @date 2023-10-11
 * @author phyliscr, morrisro
 ********************************************************************************/

#ifndef A1_MAZE_1_MODEL_CONTROLLER_H_
#define A1_MAZE_1_MODEL_CONTROLLER_H_

#include "../model/model_cave.h"
#include "../model/model_maze.h"

namespace s21 {

class Controller {
 public:
  enum class ModelType { CAVE, MAZE };

  Controller(ModelType model_type, const std::string &filename);
  Controller(uint16_t row, uint16_t column);
  Controller(uint16_t row, uint16_t column, uint16_t odds);
  ~Controller();

  Controller(const Controller &other) = delete;
  Controller &operator=(const Controller &other) = delete;

  inline uint16_t GetColumns() {
    if (model_type_ == ModelType::MAZE)
      return model_maze_->GetColumns();
    else
      return model_cave_->GetColumns();
  }

  inline uint16_t GetRows() {
    if (model_type_ == ModelType::MAZE)
      return model_maze_->GetRows();
    else
      return model_cave_->GetRows();
  }

  inline bool GetValue(uint16_t row, uint16_t column) {
    return model_cave_->GetValue(row, column);
  }

  inline bool OneStep(uint16_t live_limit, uint16_t born_limit) {
    return model_cave_->OneStep(live_limit, born_limit);
  }
  inline bool GetValueRight(uint16_t row, uint16_t column) {
    return model_maze_->GetValueRight(row, column);
  }

  inline bool GetValueBottom(uint16_t row, uint16_t column) {
    return model_maze_->GetValueBottom(row, column);
  }

  inline std::vector<std::pair<uint16_t, uint16_t>> find_path(
      std::pair<uint16_t, uint16_t> start, std::pair<uint16_t, uint16_t> end) {
    return model_maze_->find_path(start, end);
  }

 private:
  ModelType model_type_;
  ModelMaze *model_maze_;
  ModelCave *model_cave_;
};

}  // namespace s21

#endif  // A1_MAZE_1_MODEL_CONTROLLER_H_
