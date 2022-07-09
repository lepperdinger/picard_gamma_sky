#ifndef GAMMA_SKY_SRC_SKY_H
#define GAMMA_SKY_SRC_SKY_H

#include "ParameterFile.h"
#include "grids.h"
#include "tensors.h"

class Sky {
public:
  Sky(const std::vector<double> &energies,
      const tensors::tensor_4d &emissivities,
      const grids::cartesian_grid_3d &emissivity_grid,
      ParameterFile::Parameters &parameters);
  tensors::tensor_3d compute_gamma_skies();

private:
  void initialize_sky_grid();
  void initialize_relative_emissivity_grid();
  void initialize_integration_intervals();
  static void make_grid(size_t number_of_grid_points,
                        const std::array<double, 2> &grid_interval,
                        std::vector<double> &cell_boundaries,
                        std::vector<double> &cell_centers);
  static std::vector<double> make_relative_grid(const std::vector<double> &grid,
                                                double observer_location);
  static std::vector<std::array<double, 2>>
  get_integration_intervals(const std::vector<double> &cell_boundaries);

  grids::longitude_latitude_grid sky_grid;
  grids::cartesian_grid_3d relative_emissivity_grid;
  const std::vector<double> &energies;
  const tensors::tensor_4d &emissivities;
  const grids::cartesian_grid_3d &emissivity_grid;
  // {x, y, z} observer location in kpc
  std::array<double, 3> xyz_observer_location;
  // size of the radial bins in kpc
  double radial_step_size;
  // minimum and maximum of the longitudinal grid in radian
  std::array<double, 2> longitudinal_grid_interval;
  size_t number_of_longitudinal_grid_points;
  // minimum and maximum of the latitudinal grid in radian
  std::array<double, 2> latitudinal_grid_interval;
  size_t number_of_latitudinal_grid_points;
  std::vector<std::array<double, 2>> longitude_integration_intervals;
  std::vector<std::array<double, 2>> latitude_integration_intervals;
};

#endif // GAMMA_SKY_SRC_SKY_H
