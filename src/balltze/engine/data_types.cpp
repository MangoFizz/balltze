// SPDX-License-Identifier: GPL-3.0-only

#include <cmath>
#include <balltze/engine/data_types.hpp>

namespace Balltze::Engine {
    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
    Quaternion::Quaternion(const RotationMatrix &matrix) noexcept {
        float tr = matrix.v[0].x + matrix.v[1].y + matrix.v[2].z;
        if(tr > 0) {
            float S = std::sqrt(tr+1.0f) * 2.0f; // S=4*qw
            this->w = 0.25f * S;
            this->i = (matrix.v[2].y - matrix.v[1].z) / S;
            this->j = (matrix.v[0].z - matrix.v[2].x) / S;
            this->k = (matrix.v[1].x - matrix.v[0].y) / S;
        }
        else if((matrix.v[0].x > matrix.v[1].y) & (matrix.v[0].x > matrix.v[2].z)) {
            float S = std::sqrt(1.0f + matrix.v[0].x - matrix.v[1].y - matrix.v[2].z) * 2.0f; // S=4*qx
            this->w = (matrix.v[2].y - matrix.v[1].z) / S;
            this->i = 0.25f * S;
            this->j = (matrix.v[0].y + matrix.v[1].x) / S;
            this->k = (matrix.v[0].z + matrix.v[2].x) / S;
        } else if(matrix.v[1].y > matrix.v[2].z) {
            float S = std::sqrt(1.0f + matrix.v[1].y - matrix.v[0].x - matrix.v[2].z) * 2.0f; // S=4*qy
            this->w = (matrix.v[0].z - matrix.v[2].x) / S;
            this->i = (matrix.v[0].y + matrix.v[1].x) / S;
            this->j = 0.25f * S;
            this->k = (matrix.v[1].z + matrix.v[2].y) / S;
        } else {
            float S = std::sqrt(1.0f + matrix.v[2].z - matrix.v[0].x - matrix.v[1].y) * 2.0f; // S=4*qz
            this->w = (matrix.v[1].x - matrix.v[0].y) / S;
            this->i = (matrix.v[0].z + matrix.v[2].x) / S;
            this->j = (matrix.v[1].z + matrix.v[2].y) / S;
            this->k = 0.25f * S;
        }
    }

    RotationMatrix::RotationMatrix() noexcept {}

    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
    RotationMatrix::RotationMatrix(const Quaternion &quaternion) noexcept {
        float sqw = quaternion.w*quaternion.w;
        float sqx = quaternion.i*quaternion.i;
        float sqy = quaternion.j*quaternion.j;
        float sqz = quaternion.k*quaternion.k;

        // invs (inverse square length) is only required if quaternion is not already normalised
        float invs = 1.0f / (sqx + sqy + sqz + sqw);
        this->v[0].x = ( sqx - sqy - sqz + sqw)*invs; // since sqw + sqx + sqy + sqz =1/invs*invs
        this->v[1].y = (-sqx + sqy - sqz + sqw)*invs;
        this->v[2].z = (-sqx - sqy + sqz + sqw)*invs;

        float tmp1 = quaternion.i*quaternion.j;
        float tmp2 = quaternion.k*quaternion.w;
        this->v[1].x = 2.0f * (tmp1 + tmp2)*invs;
        this->v[0].y = 2.0f * (tmp1 - tmp2)*invs;

        tmp1 = quaternion.i*quaternion.k;
        tmp2 = quaternion.j*quaternion.w;
        this->v[2].x = 2.0f * (tmp1 - tmp2)*invs;
        this->v[0].z = 2.0f * (tmp1 + tmp2)*invs;
        tmp1 = quaternion.j*quaternion.k;
        tmp2 = quaternion.i*quaternion.w;
        this->v[2].y = 2.0f * (tmp1 + tmp2)*invs;
        this->v[1].z = 2.0f * (tmp1 - tmp2)*invs;
    }

	ColorARGB color_argb_32_to_real(ColorARGBInt color) noexcept {
        return {
            .alpha = static_cast<float>(color.alpha) / 255.0f,
            .red = static_cast<float>(color.red) / 255.0f,
            .green = static_cast<float>(color.green) / 255.0f,
            .blue = static_cast<float>(color.blue) / 255.0f
        };
    }
}
