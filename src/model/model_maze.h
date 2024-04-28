/*******************************************************************************
 * @copyright Copyright [2023] <Phylis Cristobal, Morris Roggo>
 * @brief A1_Maze-1.
 * @file model_maze.h
 * @date 2023-10-11
 * @author phyliscr, morrisro
 ********************************************************************************/

#ifndef A1_MAZE_1_MODEL_MODEL_MAZE_H_
#define A1_MAZE_1_MODEL_MODEL_MAZE_H_

#include <cstdint>
#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace s21 {
class ModelMaze {
 public:
  ModelMaze(size_t rows, size_t columns);
  explicit ModelMaze(const std::string &filename);
  inline size_t GetColumns() { return columns_; }
  inline size_t GetRows() { return rows_; }
  inline bool GetValueRight(size_t row, size_t column) {
    return right_walls_[row][column];
  }
  inline bool GetValueBottom(size_t row, size_t column) {
    return bottom_walls_[row][column];
  }
  std::vector<std::pair<uint16_t, uint16_t>> find_path(
      std::pair<uint16_t, uint16_t> start, std::pair<uint16_t, uint16_t> end);

 private:
  size_t rows_;
  size_t columns_;
  std::vector<std::vector<bool>> right_walls_;
  std::vector<std::vector<bool>> bottom_walls_;

  void DFS(std::pair<uint16_t, uint16_t> start,
           std::pair<uint16_t, uint16_t> end,
           std::vector<std::pair<uint16_t, uint16_t>> *path,
           std::vector<std::pair<uint16_t, uint16_t>> *result_path,
           std::map<std::pair<uint16_t, uint16_t>, bool> visited);
};
}  // namespace s21

#endif  // A1_MAZE_1_MODEL_MODEL_MAZE_H_
