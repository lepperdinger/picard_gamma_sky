// Author: Stefan Lepperdinger
#include "TrilinearInterpolation.h"
#include <cmath>

TrilinearInterpolation::TrilinearInterpolation(
    const grids::cartesian_grid_3d &grid, const tensors::tensor_3d &values)
    : x_min(grid.x_centers.front()), y_min(grid.y_centers.front()),
      z_min(grid.z_centers.front()),
      x_step_size(grid.x_centers[1] - grid.x_centers[0]),
      y_step_size(grid.y_centers[1] - grid.y_centers[0]),
      z_step_size(grid.z_centers[1] - grid.z_centers[0]), values(values) {}

double TrilinearInterpolation::operator()(std::array<double, 3> xyz_location) {
  double double_x_index = (xyz_location[0] - x_min) / x_step_size;
  double double_y_index = (xyz_location[1] - y_min) / y_step_size;
  double double_z_index = (xyz_location[2] - z_min) / z_step_size;

  // cell index
  auto x_i = static_cast<size_t>(double_x_index);
  auto y_i = static_cast<size_t>(double_y_index);
  auto z_i = static_cast<size_t>(double_z_index);

  // position within the unit cell
  double x_p = double_x_index - static_cast<double>(x_i);
  double y_p = double_y_index - static_cast<double>(y_i);
  double z_p = double_z_index - static_cast<double>(z_i);

  // see http://paulbourke.net/miscellaneous/interpolation/
  double interpolated_value =
      (values[x_i][y_i][z_i] * (1 - x_p) * (1 - y_p) * (1 - z_p) // 000
       + values[x_i + 1][y_i][z_i] * x_p * (1 - y_p) * (1 - z_p) // 100
       + values[x_i][y_i + 1][z_i] * (1 - x_p) * y_p * (1 - z_p) // 010
       + values[x_i][y_i][z_i + 1] * (1 - x_p) * (1 - y_p) * z_p // 001
       + values[x_i + 1][y_i][z_i + 1] * x_p * (1 - y_p) * z_p   // 101
       + values[x_i][y_i + 1][z_i + 1] * (1 - x_p) * y_p * z_p   // 011
       + values[x_i + 1][y_i + 1][z_i] * x_p * y_p * (1 - z_p)   // 110
       + values[x_i + 1][y_i + 1][z_i + 1] * x_p * y_p * z_p     // 111
      );

  return interpolated_value;
}
