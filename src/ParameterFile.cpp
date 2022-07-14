#include "ParameterFile.h"
#include <iostream>
#include <regex>

#define DEGREES_TO_RADIAN 0.017453292519943295

ParameterFile::ParameterFile(const std::string &file_path)
    : file_path(file_path) {}

std::string ParameterFile::get_string(const std::string &parameter_name) {
  std::string line;
  std::ifstream file(file_path);
  std::ostringstream regular_expression;
  regular_expression << "^" << parameter_name << " *= *(.*)$";
  std::regex regex(regular_expression.str());
  std::cmatch match;
  while (std::getline(file, line)) {
    bool found_line = std::regex_match(line.c_str(), match, regex);
    if (found_line) {
      return match[1];
    }
  }
  std::cerr << "error: Couldn't find the parameter '" << parameter_name
            << "' of the parameter file '" << file_path << "'.\n";
  std::exit(1);
}

int ParameterFile::get_int(const std::string &parameter_name) {
  auto parameter_string = get_string(parameter_name);
  int parameter;
  try {
    parameter = std::stoi(parameter_string);
  } catch (std::invalid_argument &invalid_argument) {
    std::cerr << "error: Parsing of the parameter '" << parameter_name
              << "' of the parameter file '" << file_path << "' failed.\n";
    std::exit(1);
  }
  return parameter;
}

double ParameterFile::get_double(const std::string &parameter_name) {
  auto parameter_string = get_string(parameter_name);
  double parameter;
  try {
    parameter = std::stod(parameter_string);
  } catch (std::invalid_argument &invalid_argument) {
    std::cerr << "error: Parsing of the parameter '" << parameter_name
              << "' of the parameter file '" << file_path << "' failed.\n";
    std::exit(1);
  }
  return parameter;
}

ParameterFile::Parameters ParameterFile::get_parameters() {
  Parameters parameters{};
  parameters.xyz_observer_location = {
      get_double("x_observer_location_in_kpc"),
      get_double("y_observer_location_in_kpc"),
      get_double("z_observer_location_in_kpc"),
  };
  parameters.radial_step_size = get_double("radial_bin_size_in_kpc");
  parameters.line_of_sight_longitude =
      get_double("line_of_sight_longitude_in_degrees") * DEGREES_TO_RADIAN;
  parameters.line_of_sight_latitude =
      get_double("line_of_sight_latitude_in_degrees") * DEGREES_TO_RADIAN;
  parameters.healpix_order = get_int("healpix_order");
  return parameters;
}
