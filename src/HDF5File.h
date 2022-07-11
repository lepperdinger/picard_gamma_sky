#ifndef GAMMA_SKY_SRC_HDF5FILE_H
#define GAMMA_SKY_SRC_HDF5FILE_H

#include "grids.h"
#include "tensors.h"
#include <hdf5.h>
#include <string>

class HDF5File {
public:
  HDF5File(const std::string &h5_file_path, char access_mode);
  ~HDF5File();
  tensors::tensor_4d read_emissivities();
  std::vector<double> read_energies();
  grids::cartesian_grid_3d read_emissivity_grid();

  void save_skies(const tensors::tensor_3d &skies);

private:
  const std::string &h5_file_path;
  hid_t file{};

  void open_file();
  void create_file();
  void close_file();
  tensors::tensor_3d read_emissivity(size_t energy_index);
  std::vector<double> read_vector_attribute(const std::string &attribute_name);
  hssize_t get_number_of_energies();
};

#endif // GAMMA_SKY_SRC_HDF5FILE_H
