#ifndef GAMMA_SKY_SRC_LINEOFSIGHTINTEGRAL_H
#define GAMMA_SKY_SRC_LINEOFSIGHTINTEGRAL_H

#include "grids.h"
#include "tensors.h"
#include <array>

class LineOfSightIntegral {
public:
  LineOfSightIntegral(double radial_step_size,
                      const std::array<double, 3> &xyz_observer_location,
                      const grids::cartesian_grid_3d &emissivity_grid,
                      const tensors::tensor_4d &emissivities);
  double operator()(const std::array<double, 2> &longitudinal_interval,
                    const std::array<double, 2> &latitudinal_interval);

private:
  double radial_step_size;
  const std::array<double, 3> &xyz_observer_location;
  const grids::cartesian_grid_3d &emissivity_grid;
  const tensors::tensor_4d &emissivities;
};

#endif // GAMMA_SKY_SRC_LINEOFSIGHTINTEGRAL_H
