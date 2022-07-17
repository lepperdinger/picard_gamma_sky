// Author: Stefan Lepperdinger
#include "mathematics.h"
#include <array>
#include <gtest/gtest.h>
#include <vector>

namespace mathematics {

TEST(mathematics, euclidean_norm) {
  double tolerance = 1e-10;
  EXPECT_NEAR(euclidean_norm({0., 0., 0.}), 0., tolerance);
  EXPECT_NEAR(euclidean_norm({0., 42.42, 0.}), 42.42, tolerance);
  EXPECT_NEAR(euclidean_norm({0., -42.42, 0.}), 42.42, tolerance);
  EXPECT_NEAR(euclidean_norm({42.42}), 42.42, tolerance);
  EXPECT_NEAR(euclidean_norm({-42.42}), 42.42, tolerance);
  EXPECT_NEAR(euclidean_norm({2., 2., 2., 2.}), 4., tolerance);
  EXPECT_NEAR(euclidean_norm({-2., 2., 2., 2.}), 4., tolerance);
  EXPECT_NEAR(euclidean_norm({-2., -2., -2., 2.}), 4., tolerance);
  EXPECT_NEAR(euclidean_norm({-2., -2., -2., -2.}), 4., tolerance);
  EXPECT_NEAR(euclidean_norm({-2., 0., -2., -2., 0., 0., 2.}), 4., tolerance);
}

TEST(mathematics, spherical_to_cartesian) {
  double tolerance = 1e-10;
  const double degrees_to_radian = 0.017453292519943295;
  double longitude, latitude, radius;
  std::array<double, 3> result{};
  const size_t x = 0, y = 1, z = 2;

  radius = 0.;
  longitude = 0.;
  latitude = 0.;
  result = spherical_to_cartesian(radius, longitude, latitude);
  EXPECT_NEAR(result[x], 0., tolerance);
  EXPECT_NEAR(result[y], 0., tolerance);
  EXPECT_NEAR(result[z], 0., tolerance);

  radius = 42.;
  longitude = 0.;
  latitude = 0.;
  result = spherical_to_cartesian(radius, longitude, latitude);
  EXPECT_NEAR(result[x], 42., tolerance);
  EXPECT_NEAR(result[y], 0., tolerance);
  EXPECT_NEAR(result[z], 0., tolerance);

  radius = 42.;
  longitude = 0.;
  latitude = 90. * degrees_to_radian;
  result = spherical_to_cartesian(radius, longitude, latitude);
  EXPECT_NEAR(result[x], 0, tolerance);
  EXPECT_NEAR(result[y], 0., tolerance);
  EXPECT_NEAR(result[z], 42., tolerance);

  radius = 42.;
  longitude = 0.;
  latitude = -90. * degrees_to_radian;
  result = spherical_to_cartesian(radius, longitude, latitude);
  EXPECT_NEAR(result[x], 0, tolerance);
  EXPECT_NEAR(result[y], 0., tolerance);
  EXPECT_NEAR(result[z], -42., tolerance);

  radius = 42.;
  longitude = 90. * degrees_to_radian;
  latitude = 0.;
  result = spherical_to_cartesian(radius, longitude, latitude);
  EXPECT_NEAR(result[x], 0., tolerance);
  EXPECT_NEAR(result[y], 42., tolerance);
  EXPECT_NEAR(result[z], 0., tolerance);

  radius = 42.;
  longitude = 270. * degrees_to_radian;
  latitude = 0.;
  result = spherical_to_cartesian(radius, longitude, latitude);
  EXPECT_NEAR(result[x], 0., tolerance);
  EXPECT_NEAR(result[y], -42., tolerance);
  EXPECT_NEAR(result[z], 0., tolerance);

  radius = 42.;
  longitude = 180. * degrees_to_radian;
  latitude = 0.;
  result = spherical_to_cartesian(radius, longitude, latitude);
  EXPECT_NEAR(result[x], -42., tolerance);
  EXPECT_NEAR(result[y], 0., tolerance);
  EXPECT_NEAR(result[z], 0., tolerance);

  tolerance = 1e-7;
  radius = 42.;
  longitude = 23.15 * degrees_to_radian;
  latitude = 47.58 * degrees_to_radian;
  result = spherical_to_cartesian(radius, longitude, latitude);
  EXPECT_NEAR(result[x], 26.0502356, tolerance);
  EXPECT_NEAR(result[y], 11.13824614, tolerance);
  EXPECT_NEAR(result[z], 31.00523663, tolerance);
}

} // namespace mathematics
