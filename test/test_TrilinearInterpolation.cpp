#include <gtest/gtest.h>
#include <array>
#include "grids.h"
#include "tensors.h"
#include "TrilinearInterpolation.h"

namespace test_TrilinearInterpolation {

double linear_scalar_field(const std::array<double, 3> &xyz) {
  double c0 = -3.42;
  double cx = 0.435;
  double cy = 3.21;
  double cz = -.3;
  return c0 + cx * xyz[0] + cy * xyz[1] + cz * xyz[2];
}

std::vector<double> create_1d_grid(const std::array<double, 2> &interval,
                                   unsigned int number_of_points) {
  std::vector<double> grid;
  grid.reserve(number_of_points);
  double step_size = (interval[1] - interval[0]) / (number_of_points - 1);
  for (unsigned int i{0}; i != number_of_points; ++i) {
    grid.push_back(interval[0] + i * step_size);
  }
  return grid;
}

grids::cartesian_grid_3d create_3d_grid() {
  grids::cartesian_grid_3d grid;
  grid.x_centers = create_1d_grid({-12.3, 6.2}, 5);
  grid.y_centers = create_1d_grid({-10., 10.}, 100);
  grid.z_centers = create_1d_grid({-21., 9.}, 19);
  return grid;
}

tensors::tensor_3d create_grid_values(const grids::cartesian_grid_3d &grid) {
  auto values = tensors::make_3d_tensor({grid.x_centers.size(),
                                         grid.y_centers.size(),
                                         grid.z_centers.size()});

  for (size_t x{}; x != grid.x_centers.size(); ++ x) {
    double x_center = grid.x_centers[x];
    for (size_t y{}; y != grid.y_centers.size(); ++y) {
      double y_center = grid.y_centers[y];
      for (size_t z{}; z != grid.z_centers.size(); ++z) {
        double z_center = grid.z_centers[z];
        values[x][y][z] = linear_scalar_field({x_center, y_center, z_center});
      }
    }
  }
  return values;
}

TEST(test_TrilinearInterpolation, linear_scalar_field) {
  auto grid = create_3d_grid();
  auto grid_values = create_grid_values(grid);
  TrilinearInterpolation interpolation(grid, grid_values);
  double tolerance = 1e-10;
  std::array<double, 3> xyz{};

  xyz = {3.93, -8.03, 0.43};
  EXPECT_NEAR(linear_scalar_field(xyz), interpolation(xyz), tolerance);

  xyz = {5.59, -3.08, -2.86};
  EXPECT_NEAR(linear_scalar_field(xyz), interpolation(xyz), tolerance);

  xyz = {-4.76, 1.44, 5.58};
  EXPECT_NEAR(linear_scalar_field(xyz), interpolation(xyz), tolerance);
}

} // namespace test_TrilinearInterpolation