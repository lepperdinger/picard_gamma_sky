// Author: Stefan Lepperdinger
#ifndef GAMMA_SKY_SRC_HDF5FILE_H
#define GAMMA_SKY_SRC_HDF5FILE_H

#include "ParameterFile.h"
#include "grids.h"
#include "tensors.h"
#include <hdf5.h>
#include <string>

/**
 * Class for reading and writing to HDF5 files.
 */
class HDF5File {
public:
  /**
   * @param h5_file_path file path of the HDF5 file
   * @param access_mode either 'r' or 'w'. Use 'r' if you want to read data from
   *                    a file and 'w' if you want to write data to a file.
   */
  HDF5File(const std::string &h5_file_path, char access_mode);
  ~HDF5File();
  /**
   * @return emissivities[energy][x][y][z] in MeV / (s sr cm³)
   */
  tensors::tensor_4d read_emissivities();
  /**
   * Reads the energies of the emissivities.
   * @return energies in MeV
   */
  std::vector<double> read_energies();
  /**
   * @return cartesian grid in kpc
   */
  grids::cartesian_grid_3d read_emissivity_grid();

  /**
   * @param skies skies[energy][pixel] in MeV / (s sr cm²)
   */
  void save_skies(const tensors::tensor_2d &skies);

  /**
   * Saves the energies of the gamma skies.
   * @param energies energies in MeV
   */
  void save_energies(const std::vector<double> &energies);

  /**
   * Saves the parameters of the parameter file.
   * @param parameters parameters of the parameter file
   */
  void save_parameters(ParameterFile::Parameters parameters);

private:
  const std::string &h5_file_path;
  hid_t file{};

  void open_file();
  void create_file();
  void close_file();
  /**
   * @param energy_index determines the energy of the emissivity
   * @return emissivity[x][y][z] in MeV / (s sr cm³)
   */
  tensors::tensor_3d read_emissivity(size_t energy_index,
                                     size_t number_of_energies);
  /**
   * Reads an attribute vector from the group "/Data" of the HDF5 file.
   * @param attribute_name name of the attribute
   * @return attribute vector
   */
  std::vector<double> read_vector_attribute(const std::string &attribute_name);
  hssize_t get_number_of_energies();
  void add_string_attribute(hid_t dataset, const std::string &attribute_name,
                            const std::string &string);
  void add_unit_to_dataset(hid_t dataset, const std::string &unit) {
    add_string_attribute(dataset, "unit", unit);
  }
  void add_description_to_dataset(hid_t dataset,
                                  const std::string &description) {
    add_string_attribute(dataset, "description", description);
  }
  void save_vector(const std::vector<double> &vector, const std::string &name,
                   const std::string &unit, const std::string &description);
  void save_scalar(double scalar, const std::string &name,
                   const std::string &unit, const std::string &description);
};

#endif // GAMMA_SKY_SRC_HDF5FILE_H
