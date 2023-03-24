// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__MATH_HPP
#define BALLTZE_API__MATH_HPP

#include <vector>
#include <cstdint>

#define PI 3.14159265359
#define RADIAN(degree) degree * PI / 180.0f

namespace Balltze::Math {
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

    using Curve = std::vector<Point2D>;

    class QuadraticBezier {
    public:
        /**
         * Get curve points
         * @param points    Number of points for the curve
         * @return          Points array
         */
        Curve get_curve(std::size_t points = 100) noexcept;

        /**
         * Get an specific point of the curve
         * @param t         A number between 0 and 1
         * @param reverse   Calculate the point starting from the end of the curve
         * @return      Result point
         */
        Point2D get_point(float t, bool reverse = false) noexcept;

        /**
         * Constructor for quadratic bezier curve (First node is on 0,0)
         * @param p1        Second node
         * @param p2        Third node
         */
        QuadraticBezier(Point2D p1, Point2D p2) noexcept;

        /**
         * Default constructor
         */
        QuadraticBezier() {}
        
    private:
        /** Nodes */
        Point2D p1;
        Point2D p2;

        /**
         * Calculate a point of a linear bezier
         * @param t     Number between 0 and 1
         * @return      Result point
         */
        Point2D calculate_point(Point2D p0, Point2D p1, float t) noexcept;
    };
}

#endif
