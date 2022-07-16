#include "LineOfSightIntegral.h"
#include "grids.h"
#include "mathematics.h"
#include "tensors.h"
#include <cmath>
#include <gtest/gtest.h>

namespace LineOfSightIntegral_test {

grids::cartesian_grid_3d create_grid() {
  grids::cartesian_grid_3d grid;
  size_t number_of_points = 100;
  double x_min{-mathematics::pi};
  double x_max{mathematics::pi};
  double x_step_size =
      (x_max - x_min) / static_cast<double>(number_of_points - 1);
  grid.x_centers.reserve(number_of_points);
  for (size_t i{}; i != number_of_points; ++i) {
    double x_center = x_min + static_cast<double>(i) * x_step_size;
    grid.x_centers.push_back(x_center);
  }
  grid.y_centers = {-1., 1.};
  grid.z_centers = {-1., 1.};
  return grid;
}

tensors::tensor_3d create_grid_values(const grids::cartesian_grid_3d &grid) {
  auto grid_values = tensors::make_3d_tensor(
      {grid.x_centers.size(), grid.y_centers.size(), grid.z_centers.size()});
  double cm_to_kpc = 1. / 3.0856775814913673e+21;
  for (size_t x{}; x != grid.x_centers.size(); ++x) {
    double x_value = 8. * mathematics::sqr(sin(grid.x_centers[x]));
    x_value *= cm_to_kpc;
    for (size_t y{}; y != grid.y_centers.size(); ++y) {
      for (size_t z{}; z != grid.z_centers.size(); ++z) {
        grid_values[x][y][z] = x_value;
      }
    }
  }

  return grid_values;
}

TEST(LineOfSightIntegral, sin2) {
  double radial_step_size_in_kpc = 0.0001;
  auto grid = create_grid();
  auto grid_values = create_grid_values(grid);
  LineOfSightIntegral integral(radial_step_size_in_kpc, grid, grid_values);
  double expected = 1.;
  double tolerance = 1e-10;

  double longitude = 0.;
  double latitude = 0.;
  EXPECT_NEAR(expected, integral(longitude, latitude), tolerance);

  longitude = mathematics::pi;
  EXPECT_NEAR(expected, integral(longitude, latitude), tolerance);
}

} // namespace LineOfSightIntegral_test