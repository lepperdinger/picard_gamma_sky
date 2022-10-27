// Author: Stefan Lepperdinger
#ifndef GAMMA_SKY_SRC_TENSORS_H
#define GAMMA_SKY_SRC_TENSORS_H

#include <array>
#include <vector>

using std::size_t;

namespace tensors {

using tensor_1d = std::vector<double>;
using tensor_2d = std::vector<tensor_1d>;
using tensor_3d = std::vector<tensor_2d>;
using tensor_4d = std::vector<tensor_3d>;

tensor_2d make_2d_tensor(std::array<size_t, 2> dimensions,
                         double initialization_value = 0.);

tensor_3d make_3d_tensor(std::array<size_t, 3> dimensions,
                         double initialization_value = 0.);

tensor_4d make_4d_tensor(std::array<size_t, 4> dimensions,
                         double initialization_value = 0.);

} // namespace tensors

#endif // GAMMA_SKY_SRC_TENSORS_H