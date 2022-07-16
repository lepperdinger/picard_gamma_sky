#include "grids.h"

namespace grids {
bool cartesian_grid_3d::is_within_grid(
    const std::array<double, 3> &point) const {
  const double &x_min = x_centers.front();
  const double &y_min = y_centers.front();
  const double &z_min = z_centers.front();

  const double &x_max = x_centers.back();
  const double &y_max = y_centers.back();
  const double &z_max = z_centers.back();

  bool within_x = x_min < point[0] && point[0] < x_max;
  bool within_y = y_min < point[1] && point[1] < y_max;
  bool within_z = z_min < point[2] && point[2] < z_max;
  bool within_grid = within_x && within_y && within_z;
  return within_grid;
}
} // namespace grids