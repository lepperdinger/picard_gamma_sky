#include "HDF5File.h"
#include <iostream>

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
