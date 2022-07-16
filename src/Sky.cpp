#include "Sky.h"
#include "mathematics.h"
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
      healpix_order(parameters.healpix_order),
      line_of_sight_longitude(parameters.line_of_sight_longitude),
      line_of_sight_latitude(parameters.line_of_sight_latitude) {

  check_parameters();
  initialize_sky_pixels();
  initialize_relative_emissivity_grid();
}

void Sky::check_parameter(bool condition, const std::string &message) {
  if (!condition) {
    std::cerr << "error: " << message << '\n';
    std::exit(1);
  }
}

void Sky::check_parameters() const {
  check_parameter(emissivity_grid.is_within_grid(xyz_observer_location),
                  "The location of the observer has to be within the spatial "
                  "emissivity grid. Please check the parameters "
                  "x/y/z_observer_location_in_kpc in the parameter file.");
  check_parameter(
      radial_step_size > 0,
      "The radial step size has to be greater than 0. Please check the "
      "parameter radial_bin_size_in_kpc in the parameter file.");
  check_parameter(healpix_order >= 0,
                  "The HEALPix order has to be non-negative. Please check the "
                  "parameter healpix_order in the parameter file.");
  check_parameter(0 <= line_of_sight_longitude &&
                      line_of_sight_longitude <= mathematics::two_pi,
                  "The line of sight longitude has to be within the interval "
                  "[0°, 360°]. Please check the parameter "
                  "line_of_sight_longitude_in_degrees in the parameter file.");
  check_parameter(-mathematics::half_pi <= line_of_sight_latitude &&
                      line_of_sight_latitude <= mathematics::half_pi,
                  "The line of sight latitude has to be within the interval "
                  "[-90°, 90°]. Please check the parameter "
                  "line_of_sight_latitude_in_degrees in the parameter file.");
}

void Sky::initialize_sky_pixels() {
  Healpix_Map<double> healpix_map(healpix_order, RING);
  number_of_sky_pixels = healpix_map.Npix();
  sky_coordinates = tensors::make_2d_tensor({number_of_sky_pixels, 2});
  for (size_t i{}; i != number_of_sky_pixels; ++i) {
    auto pixel = healpix_map.pix2ang(static_cast<int>(i));
    auto longitude = pixel.phi;
    auto latitude = mathematics::half_pi - pixel.theta;

    // add the direction in which the observer looks
    longitude += line_of_sight_longitude;
    latitude += line_of_sight_latitude;

    sky_coordinates[i] = {longitude, latitude};
  }
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

tensors::tensor_2d Sky::compute_gamma_skies() {
  auto number_of_energies = energies.size();
  auto skies =
      tensors::make_2d_tensor({number_of_energies, number_of_sky_pixels});
  for (size_t energy{}; energy != number_of_energies; ++energy) {
    LineOfSightIntegral integral(radial_step_size, relative_emissivity_grid,
                                 emissivities[energy]);
    auto &sky = skies[energy];
    std::transform(std::execution::par, sky_coordinates.cbegin(),
                   sky_coordinates.cend(), sky.begin(),
                   [&](const auto &coordinates) {
                     return integral(coordinates[0], coordinates[1]);
                   });
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
