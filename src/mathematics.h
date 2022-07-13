#ifndef GAMMA_SKY_SRC_MATHEMATICS_H
#define GAMMA_SKY_SRC_MATHEMATICS_H

#include <array>
#include <vector>

namespace mathematics {

const double half_pi{1.5707963267948966};
const double four_pi{12.566370614359172};

double sqr(double x);

std::array<double, 3> spherical_to_cartesian(double radius, double longitude,
                                             double latitude);

double euclidean_norm(const std::vector<double> &vector);

} // namespace mathematics

#endif // GAMMA_SKY_SRC_MATHEMATICS_H
