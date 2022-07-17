// Author: Stefan Lepperdinger
#include "mathematics.h"
#include <cmath>

namespace mathematics {

double sqr(double x) { return x * x; }

std::array<double, 3> spherical_to_cartesian(double radius, double longitude,
                                             double latitude) {
  double xy_projection = radius * cos(latitude);
  double x = xy_projection * cos(longitude);
  double y = xy_projection * sin(longitude);
  double z = radius * sin(latitude);
  return {x, y, z};
}

double euclidean_norm(const std::vector<double> &vector) {
  double norm{};
  for (const auto &element : vector) {
    norm += sqr(element);
  }
  norm = sqrt(norm);
  return norm;
}

} // namespace mathematics
