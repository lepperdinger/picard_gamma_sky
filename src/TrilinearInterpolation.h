#ifndef GAMMA_SKY_SRC_TRILINEARINTERPOLATION_H
#define GAMMA_SKY_SRC_TRILINEARINTERPOLATION_H

#include "grids.h"
#include "tensors.h"

class TrilinearInterpolation {
public:
  TrilinearInterpolation(const grids::cartesian_grid_3d &grid,
                         const tensors::tensor_3d &values);
  double operator()(std::array<double, 3> xyz_location);

private:
  double x_min;
  double y_min;
  double z_min;
  double x_step_size;
  double y_step_size;
  double z_step_size;
  const tensors::tensor_3d &values;
};

#endif // GAMMA_SKY_SRC_TRILINEARINTERPOLATION_H
