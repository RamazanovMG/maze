/*******************************************************************************
 * @copyright Copyright [2023] <Phylis Cristobal, Morris Roggo>
 * @brief A1_Maze-1.
 * @file unit_tests.cc
 * @date 2023-10-11
 * @author phyliscr, morrisro
 ********************************************************************************/
#include "unit_tests.h"

#include <utility>

/*
tests.cave
5 5
1 0 1 0 0
0 0 1 1 0
0 0 1 0 1
0 1 1 1 1
0 0 0 1 1
*/
TEST(GeneralSuite, new_model_open_file) {
  s21::ModelCave model("tests/test.cave");
  ASSERT_EQ(model.GetRows(), 5);
  ASSERT_EQ(model.GetColumns(), 5);
  ASSERT_EQ(model.GetValue(0, 0), 1);
  ASSERT_EQ(model.GetValue(0, 1), 0);
  ASSERT_EQ(model.GetValue(0, 2), 1);
  ASSERT_EQ(model.GetValue(0, 3), 0);
  ASSERT_EQ(model.GetValue(0, 4), 0);
  ASSERT_EQ(model.GetValue(1, 0), 0);
  ASSERT_EQ(model.GetValue(1, 1), 0);
  ASSERT_EQ(model.GetValue(1, 2), 1);
  ASSERT_EQ(model.GetValue(1, 3), 1);
  ASSERT_EQ(model.GetValue(1, 4), 0);
  ASSERT_EQ(model.GetValue(2, 0), 0);
  ASSERT_EQ(model.GetValue(2, 1), 0);
  ASSERT_EQ(model.GetValue(2, 2), 1);
  ASSERT_EQ(model.GetValue(2, 3), 0);
  ASSERT_EQ(model.GetValue(2, 4), 1);
  ASSERT_EQ(model.GetValue(3, 0), 0);
  ASSERT_EQ(model.GetValue(3, 1), 1);
  ASSERT_EQ(model.GetValue(3, 2), 1);
  ASSERT_EQ(model.GetValue(3, 3), 1);
  ASSERT_EQ(model.GetValue(3, 4), 1);
  ASSERT_EQ(model.GetValue(4, 0), 0);
  ASSERT_EQ(model.GetValue(4, 1), 0);
  ASSERT_EQ(model.GetValue(4, 2), 0);
  ASSERT_EQ(model.GetValue(4, 3), 1);
  ASSERT_EQ(model.GetValue(4, 4), 1);
}

TEST(GeneralSuite, new_model_generator) {
  s21::ModelCave model(50, 50, 40, true);
  ASSERT_EQ(model.GetRows(), 50);
  ASSERT_EQ(model.GetColumns(), 50);
  for (size_t i = 0; i < model.GetRows(); ++i) {
    for (size_t j = 0; j < model.GetColumns(); ++j) {
      ASSERT_EQ(model.GetValue(i, j), 1);
    }
  }
}

/*
tests.cave
5 5
1 0 1 0 0
0 0 1 1 0
0 0 1 0 1
0 1 1 1 1
0 0 0 1 1

1 1 1 1 1
1 1 0 0 1
1 1 1 1 1
1 0 0 1 1
1 1 1 1 1
*/
TEST(GeneralSuite, new_model_one_step) {
  s21::ModelCave model("tests/test.cave");
  model.OneStep(5, 4);
  ASSERT_EQ(model.GetRows(), 5);
  ASSERT_EQ(model.GetColumns(), 5);
  ASSERT_EQ(model.GetValue(0, 0), 1);
  ASSERT_EQ(model.GetValue(0, 1), 1);
  ASSERT_EQ(model.GetValue(0, 2), 1);
  ASSERT_EQ(model.GetValue(0, 3), 1);
  ASSERT_EQ(model.GetValue(0, 4), 1);
  ASSERT_EQ(model.GetValue(1, 0), 1);
  ASSERT_EQ(model.GetValue(1, 1), 1);
  ASSERT_EQ(model.GetValue(1, 2), 0);
  ASSERT_EQ(model.GetValue(1, 3), 0);
  ASSERT_EQ(model.GetValue(1, 4), 1);
  ASSERT_EQ(model.GetValue(2, 0), 1);
  ASSERT_EQ(model.GetValue(2, 1), 1);
  ASSERT_EQ(model.GetValue(2, 2), 1);
  ASSERT_EQ(model.GetValue(2, 3), 1);
  ASSERT_EQ(model.GetValue(2, 4), 1);
  ASSERT_EQ(model.GetValue(3, 0), 1);
  ASSERT_EQ(model.GetValue(3, 1), 0);
  ASSERT_EQ(model.GetValue(3, 2), 0);
  ASSERT_EQ(model.GetValue(3, 3), 1);
  ASSERT_EQ(model.GetValue(3, 4), 1);
  ASSERT_EQ(model.GetValue(4, 0), 1);
  ASSERT_EQ(model.GetValue(4, 1), 1);
  ASSERT_EQ(model.GetValue(4, 2), 1);
  ASSERT_EQ(model.GetValue(4, 3), 1);
  ASSERT_EQ(model.GetValue(4, 4), 1);
}

TEST(GeneralSuite, maze_constructor_1) {
  s21::ModelMaze model("tests/test.maze");
  ASSERT_EQ(model.GetRows(), 3);
  ASSERT_EQ(model.GetColumns(), 3);
  ASSERT_EQ(model.GetValueRight(0, 0), 0);
  ASSERT_EQ(model.GetValueRight(0, 1), 0);
  ASSERT_EQ(model.GetValueRight(0, 2), 1);
  ASSERT_EQ(model.GetValueRight(1, 0), 0);
  ASSERT_EQ(model.GetValueRight(1, 1), 0);
  ASSERT_EQ(model.GetValueRight(1, 2), 1);
  ASSERT_EQ(model.GetValueRight(2, 0), 0);
  ASSERT_EQ(model.GetValueRight(2, 1), 1);
  ASSERT_EQ(model.GetValueRight(2, 2), 1);
  ASSERT_EQ(model.GetValueBottom(0, 0), 0);
  ASSERT_EQ(model.GetValueBottom(0, 1), 0);
  ASSERT_EQ(model.GetValueBottom(0, 2), 0);
  ASSERT_EQ(model.GetValueBottom(1, 0), 1);
  ASSERT_EQ(model.GetValueBottom(1, 1), 0);
  ASSERT_EQ(model.GetValueBottom(1, 2), 1);
  ASSERT_EQ(model.GetValueBottom(2, 0), 1);
  ASSERT_EQ(model.GetValueBottom(2, 1), 1);
  ASSERT_EQ(model.GetValueBottom(2, 2), 1);
}

TEST(GeneralSuite, maze_constructor_2) {
  s21::ModelMaze model(50, 50);
  ASSERT_EQ(model.GetRows(), 50);
  ASSERT_EQ(model.GetColumns(), 50);
  auto path = model.find_path(std::make_pair(0, 0), std::make_pair(49, 49));
  ASSERT_EQ(path[0], std::make_pair(static_cast<uint16_t>(0), static_cast<uint16_t>(0)));
  ASSERT_EQ(path[path.size() - 1], std::make_pair(static_cast<uint16_t>(49), static_cast<uint16_t>(49)));
  for (size_t i = 0; i < model.GetRows(); ++i) {
    ASSERT_EQ(model.GetValueRight(i, model.GetColumns() - 1), 1);
  }

  for (size_t i = 0; i < model.GetColumns(); ++i) {
    ASSERT_EQ(model.GetValueBottom(model.GetRows() - 1, i), 1);
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
