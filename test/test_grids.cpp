#include "grids.h"
#include <gtest/gtest.h>

TEST(grids, is_within_grid) {
  grids::cartesian_grid_3d grid;
  grid.x_centers = {0.32, 0.6, 23., 32.4};
  grid.y_centers = {-32., 0.6, 23., 32.5};
  grid.z_centers = {-20., 0.6, 50., 100.42};
  EXPECT_EQ(false, grid.is_within_grid({0., 0., 0.}));
  EXPECT_EQ(true, grid.is_within_grid({23., 0., 0.}));
  EXPECT_EQ(true, grid.is_within_grid({.5, -31, 100.}));
  EXPECT_EQ(false, grid.is_within_grid({32.4, 0., 0.}));
  EXPECT_EQ(false, grid.is_within_grid({0., 32.5, 0.}));
  EXPECT_EQ(false, grid.is_within_grid({0., 0., 100.42}));
  EXPECT_EQ(false, grid.is_within_grid({0.32, 0., 0.}));
  EXPECT_EQ(false, grid.is_within_grid({0., -32., 0.}));
  EXPECT_EQ(false, grid.is_within_grid({0., 0., -20.}));
}
