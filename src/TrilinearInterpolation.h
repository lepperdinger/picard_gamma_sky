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
};

#endif // GAMMA_SKY_SRC_TRILINEARINTERPOLATION_H
