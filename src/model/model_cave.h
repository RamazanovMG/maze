/*******************************************************************************
 * @copyright Copyright [2023] <Phylis Cristobal, Morris Roggo>
 * @brief A1_Maze-1.
 * @file model_cave.h
 * @date 2023-10-17
 * @author phyliscr, morrisro
 ********************************************************************************/

#ifndef A1_MAZE_1_MODEL_MODEL_CAVE_H_
#define A1_MAZE_1_MODEL_MODEL_CAVE_H_

#include <cstdint>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace s21 {

class ModelCave {
 public:
  explicit ModelCave(const std::string &filename);
  ModelCave(uint16_t rows, uint16_t columns, uint16_t odds,
            const bool test_generator = false);
  ~ModelCave();

  inline uint16_t GetColumns() { return cols_; }
  inline uint16_t GetRows() { return rows_; }
  inline bool GetValue(uint16_t row, uint16_t column) {
    return static_cast<bool>(matrix_[row][column]);
  }
  bool OneStep(uint16_t live_limit, uint16_t born_limit);
  void Generator(const bool test_generator);

 private:
  uint16_t rows_;
  uint16_t cols_;
  uint16_t odds_;
  std::vector<std::vector<bool>> matrix_;

  void OpenFile(const std::string &filename);
  void ParsingMatrixSize(std::istringstream &input_string_stream);
};

}  // namespace s21

#endif  // A1_MAZE_1_MODEL_MODEL_CAVE_H_
