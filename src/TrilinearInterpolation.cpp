#include "TrilinearInterpolation.h"

TrilinearInterpolation::TrilinearInterpolation(
    const grids::cartesian_grid_3d &grid, const tensors::tensor_3d &values) {}

double TrilinearInterpolation::operator()(std::array<double, 3> xyz_location) {

  return 0.;
}