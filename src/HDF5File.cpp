#include "HDF5File.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>

HDF5File::HDF5File(const std::string &h5_file_path)
    : h5_file_path(h5_file_path) {
  open_file();
}

HDF5File::~HDF5File() { close_file(); }

void HDF5File::open_file() {
  file = H5Fopen(h5_file_path.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
  if (file < 0) {
    std::cerr << "error: Couldn't open the file '" << h5_file_path << "'.\n";
    std::exit(1);
  }
}

void HDF5File::close_file() {
  herr_t error = H5Fclose(file);
  if (error < 0) {
    std::cerr << "warning: An error occurred while trying to close the file '"
              << h5_file_path << "'.\n";
  }
}

tensors::tensor_3d HDF5File::read_emissivity(size_t energy_index) {
  std::ostringstream dataset_name;
  dataset_name << "/Data/total_emission_E" << energy_index;
  hid_t dataset = H5Dopen2(file, dataset_name.str().c_str(), H5P_DEFAULT);
  hid_t file_space = H5Dget_space(dataset);
  int number_of_dimensions = H5Sget_simple_extent_ndims(file_space);
  if (number_of_dimensions != 3) {
    std::cerr << "error: wrong number of dimensions of the dataset '"
              << dataset_name.str() << "'.\n";
    std::exit(1);
  }
  auto dimensions = std::make_unique<hsize_t[]>(number_of_dimensions);
  H5Sget_simple_extent_dims(file_space, dimensions.get(), nullptr);

  size_t number_of_values = dimensions[0] * dimensions[1] * dimensions[2];
  auto buffer = std::make_unique<float[]>(number_of_values);

  hid_t memory_space =
      H5Screate_simple(number_of_dimensions, dimensions.get(), nullptr);

  H5Dread(dataset, H5T_NATIVE_FLOAT, memory_space, file_space, H5P_DEFAULT,
          buffer.get());

  size_t x_dimension = dimensions[2];
  size_t y_dimension = dimensions[1];
  size_t z_dimension = dimensions[0];
  auto emissivity =
      tensors::make_3d_tensor({x_dimension, y_dimension, z_dimension});
  for (size_t x{}; x != x_dimension; ++x) {
    for (size_t y{}; y != y_dimension; ++y) {
      for (size_t z{}; z != z_dimension; ++z) {
        size_t data_index = z * y_dimension * x_dimension;
        data_index += y * x_dimension;
        data_index += x;
        emissivity[x][y][z] = buffer[data_index];
      }
    }
  }

  H5Sclose(memory_space);
  H5Sclose(file_space);
  H5Dclose(dataset);
  return emissivity;
}

tensors::tensor_4d HDF5File::read_emissivities() {
  hssize_t number_of_energies = get_number_of_energies();
  tensors::tensor_4d emissivities;
  for (hssize_t energy{}; energy != number_of_energies; ++energy) {
	auto emissivity = read_emissivity(energy);
	emissivities.push_back(emissivity);
  }
  return emissivities;
}

hssize_t HDF5File::get_number_of_energies() {
  std::string group_name = "/Data";
  hid_t group = H5Gopen2(file, group_name.c_str(), H5P_DEFAULT);

  std::string attribute_name = "RadiationEnergies";
  hid_t attribute = H5Aopen(group, attribute_name.c_str(), H5P_DEFAULT);

  hid_t attribute_space = H5Aget_space(attribute);
  hssize_t number_of_energies = H5Sget_simple_extent_npoints(attribute_space);

  H5Sclose(attribute_space);
  H5Aclose(attribute);
  H5Gclose(group);

  return number_of_energies;
}

std::vector<double> HDF5File::read_energies() {
  std::string group_name = "/Data";
  hid_t group = H5Gopen2(file, group_name.c_str(), H5P_DEFAULT);

  std::string attribute_name = "RadiationEnergies";
  hid_t attribute = H5Aopen(group, attribute_name.c_str(), H5P_DEFAULT);

  hid_t attribute_space = H5Aget_space(attribute);
  hssize_t number_of_energies = H5Sget_simple_extent_npoints(attribute_space);

  auto buffer = std::make_unique<float[]>(number_of_energies);
  herr_t error = H5Aread(attribute, H5T_NATIVE_FLOAT, buffer.get());
  assert(error >= 0);

  std::vector<double> energies;
  for (hssize_t i{}; i != number_of_energies; ++i) {
    energies.push_back(buffer[i]);
  }

  H5Sclose(attribute_space);
  H5Aclose(attribute);
  H5Gclose(group);

  return energies;
}
