// Author: Stefan Lepperdinger
#ifndef GAMMA_SKY_SRC_TRILINEARINTERPOLATION_H
#define GAMMA_SKY_SRC_TRILINEARINTERPOLATION_H

#include "grids.h"
#include "tensors.h"

using std::size_t;

class TrilinearInterpolation {
public:
  /**
   * @param grid linear (!) cartesian grid
   * @param values values[x][y][z] at the grid points
   */
  TrilinearInterpolation(const grids::cartesian_grid_3d &grid,
                         const tensors::tensor_3d &values);
  /**
   * Interpolates the value at the specified location.
   * @param xyz_location interpolation location
   * @return interpolated value
   */
  double operator()(std::array<double, 3> xyz_location);

private:
  double x_min;
  double y_min;
  double z_min;
  double x_step_size;
  double y_step_size;
  double z_step_size;
  // values[x][y][z] at the grid points
  const tensors::tensor_3d &values;
};

#endif // GAMMA_SKY_SRC_TRILINEARINTERPOLATION_H
