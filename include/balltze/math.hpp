// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__MATH_HPP
#define BALLTZE_API__MATH_HPP

#include <vector>
#include <cstdint>

namespace Balltze::Math {
    #define PI 3.14159265359
    #define RADIAN(degree) degree * PI / 180.0f

    using Point = float;

    struct Point2D {
        Point x = 0.0f;
        Point y = 0.0f;

        inline Point2D operator+(Point2D const &point) {
            return {
                this->x + point.x,
                this->y + point.y
            };
        }

        friend bool operator==(const Point2D &point_a, const Point2D &point_b) {
            return point_a.x == point_b.x && point_a.y == point_b.y;
        }

        friend bool operator!=(const Point2D &point_a, const Point2D &point_b) {
            return point_a.x != point_b.x || point_a.y != point_b.y;
        }
    };
}

#endif
