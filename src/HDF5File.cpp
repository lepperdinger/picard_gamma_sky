// Author: Stefan Lepperdinger
#include "HDF5File.h"
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>

HDF5File::HDF5File(const std::string &h5_file_path, char access_mode)
    : h5_file_path(h5_file_path) {
  if (access_mode == 'r') {
    open_file();
  } else if (access_mode == 'w') {
    create_file();
  } else {
    std::ostringstream message;
    message << "HDF5File: invalid access mode '" << access_mode
            << "'. valid modes: 'r', 'w'";
    throw std::invalid_argument(message.str());
  }
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

tensors::tensor_3d HDF5File::read_emissivity(size_t energy_index,
                                             size_t number_of_energies) {
  int number_of_digits = static_cast<int>(log10(number_of_energies)) + 1;
  std::ostringstream dataset_name;
  dataset_name << "/Data/total_emission_E" << std::setfill('0')
               << std::setw(number_of_digits) << energy_index;
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
    auto emissivity = read_emissivity(energy, number_of_energies);
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

std::vector<double>
HDF5File::read_vector_attribute(const std::string &attribute_name) {
  std::string group_name = "/Data";
  hid_t group = H5Gopen2(file, group_name.c_str(), H5P_DEFAULT);

  hid_t attribute = H5Aopen(group, attribute_name.c_str(), H5P_DEFAULT);

  hid_t attribute_space = H5Aget_space(attribute);
  hssize_t number_of_elements = H5Sget_simple_extent_npoints(attribute_space);

  auto buffer = std::make_unique<float[]>(number_of_elements);
  herr_t error = H5Aread(attribute, H5T_NATIVE_FLOAT, buffer.get());
  assert(error >= 0);

  std::vector<double> data;
  for (hssize_t i{}; i != number_of_elements; ++i) {
    data.push_back(buffer[i]);
  }

  H5Sclose(attribute_space);
  H5Aclose(attribute);
  H5Gclose(group);

  return data;
}

std::vector<double> HDF5File::read_energies() {
  return read_vector_attribute("RadiationEnergies");
}

grids::cartesian_grid_3d HDF5File::read_emissivity_grid() {
  grids::cartesian_grid_3d emissivity_grid;
  emissivity_grid.x_centers = read_vector_attribute("xGridCentred");
  emissivity_grid.x_boundaries = read_vector_attribute("xGridLeft");
  emissivity_grid.y_centers = read_vector_attribute("yGridCentred");
  emissivity_grid.y_boundaries = read_vector_attribute("yGridLeft");
  emissivity_grid.z_centers = read_vector_attribute("zGridCentred");
  emissivity_grid.z_boundaries = read_vector_attribute("zGridLeft");
  return emissivity_grid;
}

void HDF5File::create_file() {
  file =
      H5Fcreate(h5_file_path.c_str(), H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);
  if (file < 0) {
    std::cerr << "error: Couldn't create the file '" << h5_file_path
              << "'. Does it already exist?\n";
    std::exit(1);
  }
}

void HDF5File::save_skies(const tensors::tensor_2d &skies) {
  int number_of_dimensions = 2;
  auto dimensions = std::make_unique<hsize_t[]>(number_of_dimensions);
  dimensions[0] = skies.size();
  dimensions[1] = skies.back().size();
  hid_t data_space =
      H5Screate_simple(number_of_dimensions, dimensions.get(), nullptr);

  std::string dataset_name = "gamma ray skies";
  hid_t dataset = H5Dcreate(file, dataset_name.c_str(), H5T_NATIVE_FLOAT,
                            data_space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  size_t buffer_size = dimensions[0] * dimensions[1];
  auto buffer = std::make_unique<float[]>(buffer_size);

  for (size_t energy{}; energy != dimensions[0]; ++energy) {
    for (size_t pixel{}; pixel != dimensions[1]; ++pixel) {
      size_t index = energy * dimensions[1] + pixel;
      buffer[index] = static_cast<float>(skies[energy][pixel]);
    }
  }

  herr_t error = H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL,
                          H5P_DEFAULT, buffer.get());
  assert(error >= 0);

  add_unit_to_dataset(dataset, "MeV / (cm^2 sr s)");
  add_description_to_dataset(
      dataset, "Gamma sky fluxes at the position of the observer. Data "
               "dimensions: (energy, HEALPix pixel)");

  H5Dclose(dataset);
  H5Sclose(data_space);
}

void HDF5File::save_vector(const std::vector<double> &vector,
                           const std::string &name, const std::string &unit,
                           const std::string &description) {
  int number_of_dimensions = 1;
  auto dimensions = std::make_unique<hsize_t[]>(number_of_dimensions);
  dimensions[0] = vector.size();
  hid_t data_space =
      H5Screate_simple(number_of_dimensions, dimensions.get(), nullptr);
  hid_t dataset = H5Dcreate(file, name.c_str(), H5T_NATIVE_FLOAT, data_space,
                            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  auto buffer = std::make_unique<float[]>(vector.size());
  for (size_t i{}; i != vector.size(); ++i) {
    buffer[i] = static_cast<float>(vector[i]);
  }
  herr_t error = H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL,
                          H5P_DEFAULT, buffer.get());
  assert(error >= 0);
  add_unit_to_dataset(dataset, unit);
  add_description_to_dataset(dataset, description);
  H5Dclose(dataset);
  H5Sclose(data_space);
}

void HDF5File::save_scalar(double scalar, const std::string &name,
                           const std::string &unit,
                           const std::string &description) {
  hid_t data_space = H5Screate(H5S_SCALAR);
  hid_t dataset = H5Dcreate(file, name.c_str(), H5T_NATIVE_DOUBLE, data_space,
                            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  herr_t error = H5Dwrite(dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL,
                          H5P_DEFAULT, &scalar);
  assert(error >= 0);
  add_unit_to_dataset(dataset, unit);
  add_description_to_dataset(dataset, description);
  H5Dclose(dataset);
  H5Sclose(data_space);
}

void HDF5File::add_string_attribute(hid_t dataset,
                                    const std::string &attribute_name,
                                    const std::string &string) {
  hid_t attribute_space = H5Screate(H5S_SCALAR);
  hid_t attribute_type = H5Tcopy(H5T_C_S1);
  H5Tset_size(attribute_type, string.size());
  H5Tset_strpad(attribute_type, H5T_STR_NULLTERM);
  hid_t attribute = H5Acreate2(dataset, attribute_name.c_str(), attribute_type,
                               attribute_space, H5P_DEFAULT, H5P_DEFAULT);
  herr_t error = H5Awrite(attribute, attribute_type, string.c_str());
  assert(error >= 0);
  H5Aclose(attribute);
  H5Tclose(attribute_type);
  H5Sclose(attribute_space);
}

void HDF5File::save_energies(const std::vector<double> &energies) {
  save_vector(energies, "energies", "MeV", "energies of the gamma ray skies");
}

void HDF5File::save_parameters(ParameterFile::Parameters parameters) {
  const auto &observer = parameters.xyz_observer_location;
  const auto &longitude = parameters.line_of_sight_longitude;
  const auto &latitude = parameters.line_of_sight_latitude;
  const auto &step_size = parameters.radial_step_size;
  const auto &order = parameters.healpix_order;

  save_vector({observer.cbegin(), observer.cend()}, "xyz observer location",
              "kpc", "(x, y, z) location of the observer within the galaxy");
  save_vector({longitude, latitude}, "line of sight longitude, latitude",
              "radian", "the direction in which the observer looks");

  save_scalar(step_size, "radial step size", "kpc",
              "the radial step size used for the line of sight integration");
  save_scalar(order, "HEALPix order", "unitless",
              "determines the number of HEALPix pixels");
}
