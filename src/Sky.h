// Author: Stefan Lepperdinger
#ifndef GAMMA_SKY_SRC_SKY_H
#define GAMMA_SKY_SRC_SKY_H

#include "ParameterFile.h"
#include "grids.h"
#include "tensors.h"
#include <healpix_map.h>

using std::size_t;

class Sky {
public:
  Sky(const std::vector<double> &energies,
      const tensors::tensor_4d &emissivities,
      const grids::cartesian_grid_3d &emissivity_grid,
      ParameterFile::Parameters &parameters);
  tensors::tensor_2d compute_gamma_skies();

private:
  static void check_parameter(bool condition,
                              const std::string &condition_string);
  void check_parameters() const;
  void initialize_sky_pixels();
  void initialize_relative_emissivity_grid();
  /**
   * Subtracts the observer location from the grid.
   * @param grid cartesian grid
   * @param observer_location location of the observer
   * @return relative grid
   */
  static std::vector<double> make_relative_grid(const std::vector<double> &grid,
                                                double observer_location);

  // longitudes and latitudes of the sky pixels in radian
  tensors::tensor_2d sky_coordinates;
  size_t number_of_sky_pixels{};
  // the grid of the emissivities subtracted by the location of the observer in
  // kpc
  grids::cartesian_grid_3d relative_emissivity_grid;
  // energies of the emissivities in MeV
  const std::vector<double> &energies;
  // emissivities[energy][x][y][z] in MeV / (s sr cm³)
  const tensors::tensor_4d &emissivities;
  // cartesian emissivity grid in kpc
  const grids::cartesian_grid_3d &emissivity_grid;
  // {x, y, z} observer location in kpc
  std::array<double, 3> xyz_observer_location;
  // size of the radial bins in kpc
  double radial_step_size;
  // determines the number of pixels of the gamma sky
  int healpix_order{};
  // longitude of the direction in which the observer looks in radian
  double line_of_sight_longitude;
  // latitude of the direction in which the observer looks in radian
  double line_of_sight_latitude;
};

#endif // GAMMA_SKY_SRC_SKY_H
