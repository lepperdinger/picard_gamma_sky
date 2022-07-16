#ifndef GAMMA_SKY_SRC_GRIDS_H
#define GAMMA_SKY_SRC_GRIDS_H

#include <array>
#include <vector>

namespace grids {

struct cartesian_grid_3d {
  std::vector<double> x_centers;
  std::vector<double> x_boundaries;

  std::vector<double> y_centers;
  std::vector<double> y_boundaries;

  std::vector<double> z_centers;
  std::vector<double> z_boundaries;

  [[nodiscard]] bool is_within_grid(const std::array<double, 3> &point) const;
};

} // namespace grids

#endif // GAMMA_SKY_SRC_GRIDS_H
