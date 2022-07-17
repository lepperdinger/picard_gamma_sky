// Author: Stefan Lepperdinger
#ifndef GAMMA_SKY_SRC_LINEOFSIGHTINTEGRAL_H
#define GAMMA_SKY_SRC_LINEOFSIGHTINTEGRAL_H

#include "TrilinearInterpolation.h"
#include "grids.h"
#include "tensors.h"
#include <array>

class LineOfSightIntegral {
public:
  /**
   * @param radial_step_size radial step size
   * @param grid linear cartesian grid
   * @param values values[x][y][z] at the cartesian grid points
   */
  LineOfSightIntegral(double radial_step_size,
                      const grids::cartesian_grid_3d &grid,
                      const tensors::tensor_3d &values);
  /**
   * Evaluates the integral \int dr r² emissivity / (4 pi r²) at the specified
   * longitude and latitude.
   * @param longitude longitude at which the integral should be evaluated
   * @param latitude latitude at which the integral should be evaluated
   * @return integral
   */
  double operator()(double longitude, double latitude);

private:
  // radial step sice in kpc
  double radial_step_size;
  // cartesian grid in kpc
  const grids::cartesian_grid_3d &grid;
  // radial centers of the cells in kpc
  std::vector<double> radial_cell_centers;
  TrilinearInterpolation interpolation;
  // integral = (integration factor) x (sum of radial cells)
  double integration_factor{};

  void initialize_radial_cells();
  void initialize_integration_factor();
};

#endif // GAMMA_SKY_SRC_LINEOFSIGHTINTEGRAL_H
