#include "HDF5File.h"
#include "ParameterFile.h"
#include "Sky.h"
#include "tensors.h"
#include <iostream>

int main(int argc, char *argv[]) {
  std::string usage =
      "usage: gamma_rays <parameter file> <input H5 file> <output H5 file>";
  if (argc != 4) {
    std::cerr << usage << std::endl;
    std::exit(1);
  }

  // get arguments
  std::string parameter_file_path(argv[1]);
  std::string input_file_path(argv[2]);
  std::string output_file_path(argv[3]);

  // open files
  HDF5File input_file(input_file_path, 'r');
  HDF5File output_file(output_file_path, 'w');
  ParameterFile parameter_file(parameter_file_path);

  // get data
  auto energies = input_file.read_energies();
  auto emissivities = input_file.read_emissivities();
  auto emissivity_grid = input_file.read_emissivity_grid();
  auto parameters = parameter_file.get_parameters();

  // compute gamma skies
  Sky sky(energies, emissivities, emissivity_grid, parameters);
  auto gamma_skies = sky.compute_gamma_skies();

  // save gamma skies
  output_file.save_skies(gamma_skies);

  return 0;
}
