// Author: Stefan Lepperdinger
#include "LineOfSightIntegral.h"
#include "mathematics.h"
#include <cmath>
#include <numeric>

LineOfSightIntegral::LineOfSightIntegral(double radial_step_size,
                                         const grids::cartesian_grid_3d &grid,
                                         const tensors::tensor_3d &values)
    : radial_step_size(radial_step_size), grid(grid),
      interpolation(grid, values) {
  initialize_radial_cells();
  initialize_integration_factor();
}

void LineOfSightIntegral::initialize_integration_factor() {
  double pc_to_m = 3.0856775814913673e16;
  double kpc_to_cm = 1e3 * 1e2 * pc_to_m;
  double radial_step_size_in_cm = radial_step_size * kpc_to_cm;
  integration_factor = radial_step_size_in_cm;
}

void LineOfSightIntegral::initialize_radial_cells() {
  double x_range = grid.x_centers.back() - grid.x_centers.front();
  double y_range = grid.y_centers.back() - grid.y_centers.front();
  double z_range = grid.z_centers.back() - grid.z_centers.front();
  double maximum_possible_distance =
      mathematics::euclidean_norm({x_range, y_range, z_range});
  auto maximum_number_of_radial_bins =
      static_cast<size_t>(maximum_possible_distance / radial_step_size);
  radial_cell_centers.reserve(maximum_number_of_radial_bins);
  double half_step_size = radial_step_size * .5;
  for (size_t i{}; i != maximum_number_of_radial_bins; ++i) {
    double radial_cell_center =
        half_step_size + static_cast<double>(i) * radial_step_size;
    radial_cell_centers.push_back(radial_cell_center);
  }
}

double LineOfSightIntegral::operator()(double longitude, double latitude) {
  std::vector<double> radial_cell_values;

  for (const auto &radius : radial_cell_centers) {
    auto cell_location =
        mathematics::spherical_to_cartesian(radius, longitude, latitude);
    if (!grid.is_within_grid(cell_location)) {
      break;
    }
    auto cell_value = interpolation(cell_location);
    radial_cell_values.push_back(cell_value);
  }

  double sum = std::accumulate(radial_cell_values.cbegin(),
                               radial_cell_values.cend(), 0.);

  double integral = integration_factor * sum;
  return integral;
}
