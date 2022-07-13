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
  integration_factor = radial_step_size_in_cm / mathematics::four_pi;
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
    double radial_cell_center = half_step_size + i * radial_step_size;
    radial_cell_centers.push_back(radial_cell_center);
  }
}

double LineOfSightIntegral::operator()(double longitude, double latitude) {
  std::vector<double> radial_cell_values;

  for (const auto &radius : radial_cell_centers) {
    auto cell_location =
        mathematics::spherical_to_cartesian(radius, longitude, latitude);
    if (!point_is_within_grid(cell_location)) {
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

bool LineOfSightIntegral::point_is_within_grid(std::array<double, 3> point) {
  const double &x_min = grid.x_centers.front();
  const double &y_min = grid.y_centers.front();
  const double &z_min = grid.z_centers.front();

  const double &x_max = grid.x_centers.back();
  const double &y_max = grid.y_centers.back();
  const double &z_max = grid.z_centers.back();

  bool within_x = x_min <= point[0] && point[0] <= x_max;
  bool within_y = y_min <= point[1] && point[1] <= y_max;
  bool within_z = z_min <= point[2] && point[2] <= z_max;
  bool within_grid = within_x && within_y && within_z;
  return within_grid;
}
