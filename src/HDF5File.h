#ifndef GAMMA_SKY_SRC_HDF5FILE_H
#define GAMMA_SKY_SRC_HDF5FILE_H

#include <hdf5.h>
#include <string>
#include "tensors.h"

class HDF5File {
public:
  HDF5File(const std::string &h5_file_path);
  ~HDF5File();
private:
  std::string h5_file_path;
  hid_t file;

  void open_file();
  void close_file();
};

#endif // GAMMA_SKY_SRC_HDF5FILE_H
