/*******************************************************************************
 * @copyright Copyright [2023] <Phylis Cristobal, Morris Roggo>
 * @brief A1_Maze-1.
 * @file model_maze.cc
 * @date 2023-10-11
 * @author phyliscr, morrisro
 ********************************************************************************/

#include "model_maze.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <sstream>

namespace s21 {
ModelMaze::ModelMaze(size_t rows, size_t columns)
    : rows_(rows), columns_(columns) {
  std::random_device rd;
  std::mt19937_64 mt(rd());
  int lower_bound = 0;
  int upper_bound = 1;
  std::uniform_int_distribution<int> coinflip(lower_bound,
                                              upper_bound);  // coinflip(mt)
  for (size_t i = 0; i < rows_; ++i) {
    right_walls_.push_back(std::vector<bool>(columns_));
    bottom_walls_.push_back(std::vector<bool>(columns_));
  }
  std::vector<std::vector<int>> set_matrix;  // матрица множеств
  for (size_t i = 0; i < rows_; ++i)
    set_matrix.push_back(std::vector<int>(columns_));

  for (size_t i = 0; i < rows_;
       ++i) {  // все ячейки без нижней границы "падают" вниз
    if (i != 0) {
      for (size_t j = 0; j < columns_; ++j) {
        if (bottom_walls_[i - 1][j] == 0)
          set_matrix[i][j] = set_matrix[i - 1][j];
      }
    }

    for (size_t j = 0; j < columns_;
         ++j) {  // все ячейки без множеств инициализируются уникальным
      // множеством
      if (set_matrix[i][j] == 0) set_matrix[i][j] = 1 + j + i * columns_;
    }

    for (size_t j = 0; j < columns_ - 1; ++j) {
      if (set_matrix[i][j] != set_matrix[i][j + 1]) {
        if (coinflip(
                mt)) {  // для разных множеств коинфлипаем правую перегородку
          right_walls_[i][j] = 1;
        } else {  // если перегородки нет, то объединяем множества в одно
          int new_value = std::min(set_matrix[i][j], set_matrix[i][j + 1]);
          int old_value = std::max(set_matrix[i][j], set_matrix[i][j + 1]);
          for (size_t k = 0; k < columns_; ++k) {
            if (set_matrix[i][k] == old_value) set_matrix[i][k] = new_value;
          }
        }
      } else {  // для ячеек в одинаковом множестве создаем перегородку, чтобы
        // избежать зацикливания
        right_walls_[i][j] = 1;
      }
    }
    right_walls_[i][columns_ - 1] = 1;  // закрываем лабиринт справа

    for (size_t j = 0; j < columns_; ++j)
      bottom_walls_[i][j] =
          coinflip(mt);  // случайно расставляем нижние перегородки

    for (size_t j = 0; j < columns_;
         ++j) {  // Для каждого закрытого множества разрушаем случайную нижнюю
      // перегородку
      int current_set = set_matrix[i][j];
      bool is_current_set_isolated = true;
      std::vector<size_t> current_set_indices;
      for (size_t k = 0; k < columns_; ++k) {
        if (set_matrix[i][k] == current_set) {
          if (bottom_walls_[i][k] == 0) is_current_set_isolated = false;
          current_set_indices.push_back(k);
        }
      }
      if (is_current_set_isolated) {
        std::uniform_int_distribution<int> random_bottom_wall_for_deletion(
            0, static_cast<int>(current_set_indices.size() - 1));
        bottom_walls_[i][current_set_indices[random_bottom_wall_for_deletion(
            mt)]] = 0;
      }
    }

    if (i ==
        rows_ - 1) {  // в последней строке разрушаем правые перегородки для
      // разных множеств, чтобы избежать закрытых комнат
      for (size_t j = 0; j < columns_ - 1; ++j) {
        if (set_matrix[i][j] != set_matrix[i][j + 1]) {
          right_walls_[i][j] = 0;
          int new_value = std::min(set_matrix[i][j], set_matrix[i][j + 1]);
          int old_value = std::max(set_matrix[i][j], set_matrix[i][j + 1]);
          for (size_t k = 0; k < columns_; ++k) {
            if (set_matrix[i][k] == old_value) set_matrix[i][k] = new_value;
          }
        }
      }
      for (size_t j = 0; j < columns_; ++j)
        bottom_walls_[i][j] = 1;  // закрываем лабиринт снизу
    }
  }
}

ModelMaze::ModelMaze(const std::string &filename) {
  std::ifstream loaded_file(filename);
  if (!loaded_file) throw std::runtime_error("Can't open specified file");
  try {
    std::string line;
    std::getline(loaded_file, line);
    std::stringstream parameters(line);
    std::string segment;
    std::getline(parameters, segment, ' ');
    rows_ = std::stoi(segment);
    std::getline(parameters, segment, ' ');
    columns_ = std::stoi(segment);

    for (size_t i = 0; i < rows_; ++i) {
      std::getline(loaded_file, line);
      std::stringstream local_walls(line);
      right_walls_.push_back(std::vector<bool>(columns_));
      for (size_t j = 0; j < columns_; ++j) {
        std::getline(local_walls, segment, ' ');
        right_walls_[i][j] = std::stoi(segment);
      }
    }
    std::getline(loaded_file, line);
    for (size_t i = 0; i < rows_; ++i) {
      std::getline(loaded_file, line);
      std::stringstream local_walls(line);
      bottom_walls_.push_back(std::vector<bool>(columns_));
      for (size_t j = 0; j < columns_; ++j) {
        std::getline(local_walls, segment, ' ');
        bottom_walls_[i][j] = std::stoi(segment);
      }
    }
  } catch (const std::invalid_argument &e) {
    throw std::invalid_argument("Parsing error");
  }
}

std::vector<std::pair<uint16_t, uint16_t>> ModelMaze::find_path(
    std::pair<uint16_t, uint16_t> start, std::pair<uint16_t, uint16_t> end) {
  if (start.first >= rows_ || start.second >= columns_ || end.first >= rows_ ||
      end.second >= columns_)
    throw std::invalid_argument("The point is outside the maze");
  std::vector<std::pair<uint16_t, uint16_t>> path;
  std::vector<std::pair<uint16_t, uint16_t>> result_path;
  std::map<std::pair<uint16_t, uint16_t>, bool> visited;
  DFS(start, end, &path, &result_path, visited);
  return result_path;
}

void ModelMaze::DFS(std::pair<uint16_t, uint16_t> start,
                    std::pair<uint16_t, uint16_t> end,
                    std::vector<std::pair<uint16_t, uint16_t>> *path,
                    std::vector<std::pair<uint16_t, uint16_t>> *result_path,
                    std::map<std::pair<uint16_t, uint16_t>, bool> visited) {
  visited[start] = true;
  path->push_back(start);

  if (start == end) {
    for (size_t i = 0; i < path->size(); ++i)
      result_path->push_back(path->at(i));
    return;
  }

  if (result_path->size() > 0) return;

  size_t i = static_cast<size_t>(start.first);
  size_t j = static_cast<size_t>(start.second);

  if (i > 0 && bottom_walls_[i - 1][j] == 0 &&
      !visited[std::make_pair(i - 1, j)])
    DFS(std::make_pair(i - 1, j), end, path, result_path, visited);
  if (j < columns_ - 1 && right_walls_[i][j] == 0 &&
      !visited[std::make_pair(i, j + 1)])
    DFS(std::make_pair(i, j + 1), end, path, result_path, visited);
  if (i < rows_ - 1 && bottom_walls_[i][j] == 0 &&
      !visited[std::make_pair(i + 1, j)])
    DFS(std::make_pair(i + 1, j), end, path, result_path, visited);
  if (j > 0 && right_walls_[i][j - 1] == 0 &&
      !visited[std::make_pair(i, j - 1)])
    DFS(std::make_pair(i, j - 1), end, path, result_path, visited);

  path->pop_back();
  return;
}

}  // namespace s21
