#ifndef GAMMA_SKY_SRC_GRIDS_H
#define GAMMA_SKY_SRC_GRIDS_H

#include <vector>

namespace grids {

struct cartesian_grid_3d {
  std::vector<double> x_centers;
  std::vector<double> x_boundaries;

  std::vector<double> y_centers;
  std::vector<double> y_boundaries;

  std::vector<double> z_centers;
  std::vector<double> z_boundaries;
};

struct longitude_latitude_grid {
  std::vector<double> longitude_centers;
  std::vector<double> longitude_boundaries;

  std::vector<double> latitude_centers;
  std::vector<double> latitude_boundaries;
};

} // namespace grids

#endif // GAMMA_SKY_SRC_GRIDS_H
