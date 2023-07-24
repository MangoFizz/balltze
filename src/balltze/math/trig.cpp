// SPDX-License-Identifier: GPL-3.0-only

#include <cmath>
#include <balltze/math.hpp>

namespace Balltze::Math {
    // special thanks to MosesOfEgypt for the rotation interpolation stuff here
    void interpolate_quat(const Quaternion &in_before, const Quaternion &in_after, Quaternion &out, float scale) noexcept {
        auto &w1 = in_before.w;
        auto &x1 = in_before.i;
        auto &y1 = in_before.j;
        auto &z1 = in_before.k;
        auto w0 = in_after.w;
        auto x0 = in_after.i;
        auto y0 = in_after.j;
        auto z0 = in_after.k;
        float cos_half_theta = w0*w1 + x0*x1 + y0*y1 + z0*z1;
        if(cos_half_theta < 0) {
            w0*=-1;
            x0*=-1;
            y0*=-1;
            z0*=-1;
            cos_half_theta *= -1;
        }
        if(cos_half_theta < 0.01) return;

        float half_theta;
        if(std::fabs(cos_half_theta) >= 1.0) {
            half_theta = 0.0;
        }
        else {
            half_theta = std::acos(cos_half_theta);
        }

        float sin_half_theta = 0;
        float m = (1 - cos_half_theta*cos_half_theta);
        if(m > 0) sin_half_theta = m;

        float r0 = 1 - scale;
        float r1 = scale;
        if(sin_half_theta > 0.00001) {
            r0 = std::sin((1 - scale) * half_theta) / sin_half_theta;
            r1 = std::sin(scale * half_theta) / sin_half_theta;
        }

        out.w = w0*r1 + w1*r0;
        out.i = x0*r1 + x1*r0;
        out.j = y0*r1 + y1*r0;
        out.k = z0*r1 + z1*r0;
    }

    void interpolate_point(const Point3D &before, const Point3D &after, Point3D &output, float scale) noexcept {
        output.x = before.x + (after.x - before.x) * scale;
        output.y = before.y + (after.y - before.y) * scale;
        output.z = before.z + (after.z - before.z) * scale;
    }

    float distance_squared(float x1, float y1, float x2, float y2) noexcept {
        float x = x1 - x2;
        float y = y1 - y2;
        return x*x + y*y;
    }

    float distance_squared(float x1, float y1, float z1, float x2, float y2, float z2) noexcept {
        float x = x1 - x2;
        float y = y1 - y2;
        float z = z1 - z2;
        return x*x + y*y + z*z;
    }

    float distance_squared(const Point3D &a, const Point3D &b) noexcept {
        return distance_squared(a.x, a.y, a.z, b.x, b.y, b.z);
    }

    float distance_squared(const Point2D &a, const Point2D &b) noexcept {
        return distance_squared(a.x, a.y, b.x, b.y);
    }

    float distance(float x1, float y1, float x2, float y2) noexcept {
        return std::sqrt(distance_squared(x1, y1, x2, y2));
    }

    float distance(float x1, float y1, float z1, float x2, float y2, float z2) noexcept {
        return std::sqrt(distance_squared(x1, y1, z1, x2, y2, z2));
    }

    float distance(const Point3D &a, const Point3D &b) noexcept {
        return std::sqrt(distance_squared(a.x, a.y, a.z, b.x, b.y, b.z));
    }

    float distance(const Point2D &a, const Point3D &b) noexcept {
        return std::sqrt(distance_squared(a.x, a.y, b.x, b.y));
    }
}
