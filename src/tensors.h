#ifndef GAMMA_SKY_SRC_TENSORS_H
#define GAMMA_SKY_SRC_TENSORS_H

#include <array>
#include <vector>

namespace tensors {

using tensor_1d = std::vector<double>;
using tensor_2d = std::vector<tensor_1d>;
using tensor_3d = std::vector<tensor_2d>;
using tensor_4d = std::vector<tensor_3d>;

tensor_2d make_2d_tensor(std::array<size_t, 2> dimensions,
                         double initialization_value = 0.) {
  tensor_1d dimension_1(dimensions[1], initialization_value);
  tensor_2d dimension_0(dimensions[0], dimension_1);
  return dimension_0;
}

tensor_3d make_3d_tensor(std::array<size_t, 3> dimensions,
                         double initialization_value = 0.) {
  tensor_1d dimension_2(dimensions[2], initialization_value);
  tensor_2d dimension_1(dimensions[1], dimension_2);
  tensor_3d dimension_0(dimensions[0], dimension_1);
  return dimension_0;
}

tensor_4d make_4d_tensor(std::array<size_t, 4> dimensions,
                         double initialization_value = 0.) {
  tensor_1d dimension_3(dimensions[3], initialization_value);
  tensor_2d dimension_2(dimensions[2], dimension_3);
  tensor_3d dimension_1(dimensions[1], dimension_2);
  tensor_4d dimension_0(dimensions[0], dimension_1);
  return dimension_0;
}

} // namespace tensors

#endif // GAMMA_SKY_SRC_TENSORS_H