#ifndef GAMMA_SKY_SRC_SKY_H
#define GAMMA_SKY_SRC_SKY_H

#include "ParameterFile.h"
#include "grids.h"
#include "tensors.h"
#include <healpix_map.h>

class Sky {
public:
  Sky(const std::vector<double> &energies,
      const tensors::tensor_4d &emissivities,
      const grids::cartesian_grid_3d &emissivity_grid,
      ParameterFile::Parameters &parameters);
  tensors::tensor_2d compute_gamma_skies();

private:
  void initialize_sky_pixels();
  void initialize_relative_emissivity_grid();
  static std::vector<double> make_relative_grid(const std::vector<double> &grid,
                                                double observer_location);

  // longitudes and latitudes of the sky pixels
  tensors::tensor_2d sky_coordinates;
  size_t number_of_sky_pixels{};
  grids::cartesian_grid_3d relative_emissivity_grid;
  const std::vector<double> &energies;
  const tensors::tensor_4d &emissivities;
  const grids::cartesian_grid_3d &emissivity_grid;
  // {x, y, z} observer location in kpc
  std::array<double, 3> xyz_observer_location;
  // size of the radial bins in kpc
  double radial_step_size;
  int healpix_order{};
  double line_of_sight_longitude;
  double line_of_sight_latitude;
};

#endif // GAMMA_SKY_SRC_SKY_H
