/*******************************************************************************
 * @copyright Copyright [2023] <Phylis Cristobal, Morris Roggo>
 * @brief A1_Maze-1.
 * @file model_cave.cc
 * @date 2023-10-17
 * @author phyliscr, morrisro
 ********************************************************************************/

#include "model_cave.h"

namespace s21 {

ModelCave::ModelCave(const std::string &filename)
    : rows_(0), cols_(0), odds_(0) {
  OpenFile(filename);
}

ModelCave::ModelCave(uint16_t rows, uint16_t columns, uint16_t odds,
                     const bool test_generator)
    : rows_(rows), cols_(columns), odds_(odds) {
  Generator(test_generator);
}

ModelCave::~ModelCave() {}

/*!
 * \brief ModelCave::Generator Генерация с использованием клеточного автомата.
 */
void ModelCave::Generator(const bool test_generator) {
  std::random_device rd;
  std::mt19937_64 mt(rd());
  matrix_.reserve(rows_);
  for (uint16_t row = 0; row < rows_; ++row) {
    matrix_.push_back(std::vector<bool>());
    matrix_[row].reserve(cols_);
    for (uint16_t col = 0; col < cols_; ++col) {
      std::uniform_int_distribution<int> distribution(1, 100);
      if (test_generator)
        matrix_[row].push_back(true);
      else
        matrix_[row].push_back(distribution(mt) <= odds_ ? true : false);
    }
  }
}

/*!
 * \brief ModelCave::OpenFile Открытие файла содержащего лабиринт.
 * \param filename Абсолютный путь к файлу.
 */
void ModelCave::OpenFile(const std::string &filename) {
  std::ifstream inputFile(filename);

  if (inputFile.is_open()) {
    std::string line;
    std::getline(inputFile, line);
    std::istringstream input_string_stream(line);

    try {
      ParsingMatrixSize(input_string_stream);
      matrix_.reserve(rows_);

      for (uint16_t row = 0; row < rows_; ++row) {
        std::getline(inputFile, line);
        input_string_stream.clear();
        input_string_stream.str(line);

        uint16_t num_buffer;
        matrix_.push_back(std::vector<bool>());
        matrix_[row].reserve(cols_);
        while (input_string_stream >> num_buffer) {
          matrix_[row].push_back(num_buffer);
        }
        if (matrix_[row].size() != cols_)
          throw std::runtime_error("Ошибка чтения файла.");
      }
    } catch (const std::runtime_error &e) {
      throw std::runtime_error("Ошибка [ParsingMatrixSize]: " +
                               std::string(e.what()));
    }
  } else {
    throw std::runtime_error("Ошибка открытия файла.");
  }
}

/*!
 * \brief ModelCave::ParsingMatrixSize Обработка первой строки файла с размером
 * матриц. \param std::istringstream Ссылка на строковый поток. \return Статус
 * выполнения.
 */
void ModelCave::ParsingMatrixSize(std::istringstream &input_string_stream) {
  uint16_t rows = 0;
  if (input_string_stream >> rows) {
    uint16_t cols = 0;
    if (input_string_stream >> cols) {
      if (rows >= 2 && cols >= 2 && rows <= 50 && cols <= 50) {
        rows_ = rows;
        cols_ = cols;
      } else {
        rows_ = 0;
        cols_ = 0;
        throw std::runtime_error(
            "Количество столбцов или строк не удовлетворяет заданным "
            "условиям или равно нулю. [" +
            std::to_string(rows_) + "," + std::to_string(cols_) + "]");
      }
    }
  }
}

/*!
 * \brief ModelCave::OneStep TakeOneStep Функция для выполнения одного шага
 * клеточного автомата. Если "живые" клетки окружены "живыми" клетками,
 * количество которых меньше, чем предел "смерти", они "умирают". Если "мертвые"
 * клетки находятся рядом с "живыми", количество которых больше, чем предел
 * "рождения", они становятся "живыми".
 */
bool ModelCave::OneStep(uint16_t live_limit, uint16_t born_limit) {
  std::vector<std::vector<bool>> newMatrix(rows_, std::vector<bool>(cols_, 0));
  bool return_flag = true;
  for (uint16_t row = 0; row < rows_; ++row) {
    for (uint16_t col = 0; col < cols_; ++col) {
      uint16_t neighbors = 0;

      // Подсчет количества соседей
      for (int16_t n_row = -1; n_row <= 1; ++n_row) {
        for (int16_t n_col = -1; n_col <= 1; ++n_col) {
          if (n_row == 0 && n_col == 0) continue;  // Исключаем текущую клетку
          int16_t n_row_ = row + n_row;
          int16_t n_col_ = col + n_col;

          if (n_row_ >= 0 && n_row_ < rows_ && n_col_ >= 0 && n_col_ < cols_)
            neighbors += matrix_[n_row_][n_col_];
          else  // Клетки за границей пещеры считаются живыми
            ++neighbors;
        }
      }
      // Применение правил клеточного автомата
      if (matrix_[row][col] == 1) {  // "живая" клетка
        neighbors < live_limit ? newMatrix[row][col] = 0
                               : newMatrix[row][col] = 1;
      } else {  // "мертвая" клетка
        if (neighbors >= born_limit) newMatrix[row][col] = 1;
      }
    }
  }
  for (uint16_t row = 0; row < rows_; ++row) {
    for (uint16_t col = 0; col < cols_; ++col) {
      if (newMatrix[row][col] != matrix_[row][col]) {
        return_flag = false;
      }
    }
  }
  matrix_ = newMatrix;
  return return_flag;
}

}  // namespace s21
