// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/math.hpp>

namespace Balltze::Math {
    Point2D QuadraticBezier::calculate_point(Point2D p0, Point2D p1, float t) const noexcept {
        return {
            p0.x + (p0.x - p1.x) * t,
            p0.y + (p0.y - p1.y) * t
        };
    }

    Curve QuadraticBezier::get_curve(std::size_t points) const noexcept {
        Point2D p0, p1, p2;
        Curve curve;
        p1 = this->p1;
        p2 = this->p2;

        float tp = 1.0f / (points - 1);
        for(float i = 0; i < points; i++) {
            float t = tp * i;
            auto q0 = this->calculate_point(p0, p1, t);
            auto q1 = this->calculate_point(p1, p2, t);
            auto r = this->calculate_point(q0, q1, t);
            curve.push_back(r);
        }
        return curve;
    }

    Point2D QuadraticBezier::get_point(float t, bool reverse) const noexcept {
        Point2D p0, p1, p2;
        p1 = this->p1;
        p2 = this->p2;

        // If reverse flag is set then we start from the end of the curve
        if(reverse) {
            t = 1 - t;
        }

        auto q0 = this->calculate_point(p0, p1, t);
        auto q1 = this->calculate_point(p1, p2, t);
        auto r = this->calculate_point(q0, q1, t);
        return r;
    }

    QuadraticBezier::QuadraticBezier(Point2D p1, Point2D p2) noexcept {
        this->p1 = p1;
        this->p2 = p2;
    }

    QuadraticBezier QuadraticBezier::flat() noexcept {
        return QuadraticBezier({0.0f, 0.0f}, {0.0f, 0.0f});
    }

    QuadraticBezier QuadraticBezier::linear() noexcept {
        return QuadraticBezier({0.0f, 0.0f}, {1.0f, 1.0f});
    }

    QuadraticBezier QuadraticBezier::ease_in() noexcept {
        return QuadraticBezier({0.4f, 0.0f}, {1.0f, 1.0f});
    }

    QuadraticBezier QuadraticBezier::ease_out() noexcept {
        return QuadraticBezier({0.0f, 0.0f}, {0.6f, 1.0f});
    }

    QuadraticBezier QuadraticBezier::ease_in_out() noexcept {
        return QuadraticBezier({0.4f, 0.0f}, {0.6f, 1.0f});
    }
}