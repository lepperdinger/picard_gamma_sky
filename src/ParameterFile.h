#ifndef GAMMA_SKY_SRC_PARAMETERFILE_H
#define GAMMA_SKY_SRC_PARAMETERFILE_H

#include <array>
#include <fstream>
#include <string>

class ParameterFile {
public:
  struct Parameters {
    // {x, y, z} observer location in kpc
    std::array<double, 3> xyz_observer_location;
    // size of the radial bins in kpc
    double radial_step_size;
    // minimum and maximum of the longitudinal grid in radian
    std::array<double, 2> longitudinal_grid_interval;
    int number_of_longitudinal_grid_points;
    // minimum and maximum of the latitudinal grid in radian
    std::array<double, 2> latitudinal_grid_interval;
    int number_of_latitudinal_grid_points;
  };
  explicit ParameterFile(const std::string &file_path);
  Parameters get_parameters();

private:
  std::string get_string(const std::string &parameter_name);
  int get_int(const std::string &parameter_name);
  double get_double(const std::string &parameter_name);
  const std::string &file_path;
};

#endif // GAMMA_SKY_SRC_PARAMETERFILE_H
