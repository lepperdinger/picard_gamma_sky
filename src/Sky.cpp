#include "Sky.h"
#include <LineOfSightIntegral.h>
#include <algorithm>
#include <execution>

Sky::Sky(const std::vector<double> &energies,
         const tensors::tensor_4d &emissivities,
         const grids::cartesian_grid_3d &emissivity_grid,
         ParameterFile::Parameters &parameters)
    : energies(energies), emissivities(emissivities),
      emissivity_grid(emissivity_grid),
      xyz_observer_location(parameters.xyz_observer_location),
      radial_step_size(parameters.radial_step_size),
      longitudinal_grid_interval(parameters.longitudinal_grid_interval),
      number_of_longitudinal_grid_points(
          parameters.number_of_longitudinal_grid_points),
      latitudinal_grid_interval(parameters.latitudinal_grid_interval),
      number_of_latitudinal_grid_points(
          parameters.number_of_latitudinal_grid_points)

{
  initialize_sky_grid();
  initialize_relative_emissivity_grid();
  initialize_integration_intervals();
}

void Sky::initialize_sky_grid() {
  make_grid(number_of_longitudinal_grid_points, longitudinal_grid_interval,
            sky_grid.longitude_boundaries, sky_grid.longitude_centers);
  make_grid(number_of_latitudinal_grid_points, latitudinal_grid_interval,
            sky_grid.latitude_boundaries, sky_grid.latitude_centers);
}

void Sky::initialize_relative_emissivity_grid() {
  relative_emissivity_grid.x_boundaries = make_relative_grid(
      emissivity_grid.x_boundaries, xyz_observer_location[0]);
  relative_emissivity_grid.x_centers =
      make_relative_grid(emissivity_grid.x_centers, xyz_observer_location[0]);

  relative_emissivity_grid.y_boundaries = make_relative_grid(
      emissivity_grid.y_boundaries, xyz_observer_location[1]);
  relative_emissivity_grid.y_centers =
      make_relative_grid(emissivity_grid.y_centers, xyz_observer_location[1]);

  relative_emissivity_grid.z_boundaries = make_relative_grid(
      emissivity_grid.z_boundaries, xyz_observer_location[2]);
  relative_emissivity_grid.z_centers =
      make_relative_grid(emissivity_grid.z_centers, xyz_observer_location[2]);
}

void Sky::initialize_integration_intervals() {
  longitude_integration_intervals =
      get_integration_intervals(sky_grid.longitude_boundaries);
  latitude_integration_intervals =
      get_integration_intervals(sky_grid.latitude_boundaries);
}

tensors::tensor_3d Sky::compute_gamma_skies() {

  const auto &longitudes = longitude_integration_intervals;
  const auto &latitudes = latitude_integration_intervals;

  auto skies = tensors::make_3d_tensor(
      {energies.size(), longitudes.size(), latitudes.size()});

  for (size_t energy{}; energy != energies.size(); ++energy) {
    LineOfSightIntegral integral(radial_step_size, xyz_observer_location,
                                 relative_emissivity_grid,
                                 emissivities[energy]);
    for (size_t x{}; x != longitudes.size(); ++x) {
      const auto &longitude = longitudes[x];
      std::transform(std::execution::par, latitudes.cbegin(), latitudes.cend(),
                     skies[energy][x].begin(),
                     [&](const std::array<double, 2> &latitude) {
                       return integral(longitude, latitude);
                     });
    }
  }
  return skies;
}

std::vector<double> Sky::make_relative_grid(const std::vector<double> &grid,
                                            double observer_location) {
  std::vector<double> relative_grid;
  relative_grid.reserve(grid.size());
  std::transform(
      grid.cbegin(), grid.cend(), std::back_inserter(relative_grid),
      [&observer_location](double x) { return x - observer_location; });
  return relative_grid;
}

std::vector<std::array<double, 2>>
Sky::get_integration_intervals(const std::vector<double> &cell_boundaries) {
  auto number_of_intervals = cell_boundaries.size() - 1;
  std::vector<std::array<double, 2>> integration_intervals;
  integration_intervals.reserve(number_of_intervals);
  for (size_t i{}; i != number_of_intervals; ++i) {
    integration_intervals.push_back(
        {cell_boundaries[i], cell_boundaries[i + 1]});
  }

  return integration_intervals;
}

void Sky::make_grid(size_t number_of_grid_points,
                    const std::array<double, 2> &grid_interval,
                    std::vector<double> &cell_boundaries,
                    std::vector<double> &cell_centers) {
  double interval_range = grid_interval[1] - grid_interval[0];
  double cell_width = interval_range / number_of_grid_points;
  double half_cell_width = cell_width * .5;
  size_t number_of_cells = number_of_grid_points;
  size_t number_of_boundaries = number_of_grid_points + 1;

  cell_boundaries.reserve(number_of_boundaries);
  for (size_t index{0}; index != number_of_boundaries; ++index) {
    double cell_boundary = grid_interval[0] + cell_width * index;
    cell_boundaries.push_back(cell_boundary);
  }

  cell_centers.reserve(number_of_cells);
  for (size_t index{0}; index != number_of_cells; ++index) {
    double cell_center =
        grid_interval[0] + half_cell_width + cell_width * index;
    cell_centers.push_back(cell_center);
  }
}