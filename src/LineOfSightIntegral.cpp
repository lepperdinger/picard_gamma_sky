#include "LineOfSightIntegral.h"

LineOfSightIntegral::LineOfSightIntegral(
    double radial_step_size, const std::array<double, 3> &xyz_observer_location,
    const grids::cartesian_grid_3d &emissivity_grid,
    const tensors::tensor_4d &emissivities)
    : radial_step_size(radial_step_size),
      xyz_observer_location(xyz_observer_location),
      emissivity_grid(emissivity_grid), emissivities(emissivities) {}

double LineOfSightIntegral::operator()(
    const std::array<double, 2> &longitudinal_interval,
    const std::array<double, 2> &latitudinal_interval) {
  return 42.;
}
