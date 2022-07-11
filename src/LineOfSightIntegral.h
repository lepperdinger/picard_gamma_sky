#ifndef GAMMA_SKY_SRC_LINEOFSIGHTINTEGRAL_H
#define GAMMA_SKY_SRC_LINEOFSIGHTINTEGRAL_H

#include "TrilinearInterpolation.h"
#include "grids.h"
#include "tensors.h"
#include <array>

class LineOfSightIntegral {
public:
  LineOfSightIntegral(double radial_step_size,
                      const grids::cartesian_grid_3d &grid,
                      const tensors::tensor_3d &values);
  double operator()(const std::array<double, 2> &longitudinal_interval,
                    const std::array<double, 2> &latitudinal_interval);

private:
  double radial_step_size;
  const grids::cartesian_grid_3d &grid;
  std::vector<double> radial_cell_centers;
  TrilinearInterpolation interpolation;

  void initialize_radial_cells();
  bool point_is_within_grid(std::array<double, 3> point);
  static double
};

#endif // GAMMA_SKY_SRC_LINEOFSIGHTINTEGRAL_H
