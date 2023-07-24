// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__MATH_HPP
#define BALLTZE_API__MATH_HPP

#include <vector>
#include <cstdint>
#include "engine/data_types.hpp"

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679
#define RADIAN(degree) degree * PI / 180.0f
#define DEGREE(radian) radian * 180.0f / PI

namespace Balltze::Math {
    using Point2D = Engine::Point2D;
    using Point3D = Engine::Point3D;
    using Quaternion = Engine::Quaternion;
    using RotationMatrix = Engine::RotationMatrix;
    using Curve = std::vector<Point2D>;

    /**
     * Interpolate a quaternion.
     * @param in_before This is the quaternion to interpolate from.
     * @param in_after  This is the quaternion to interpolate to.
     * @param out       This is the quaternion to overwrite.
     * @param progress  This is how far in between each quaternion (0.0 - 1.0) to create an interpolated quaternion.
     */
    void interpolate_quat(const Quaternion &in_before, const Quaternion &in_after, Quaternion &out, float progress) noexcept;

    /**
     * Interpolate a 3D point.
     * @param in_before This is the 3D point to interpolate from.
     * @param in_after  This is the 3D point to interpolate to.
     * @param out       This is the 3D point to overwrite.
     * @param progress  This is how far in between each point (0.0 - 1.0) to create an interpolated 3D point.
     */
    void interpolate_point(const Point3D &before, const Point3D &after, Point3D &output, float scale) noexcept;

    /**
     * Calculate the distance between two 2D points without taking square roots. If the square root isn't necessary, then this is faster.
     * @param  x1 This is the X coordinate of the first point.
     * @param  y1 This is the Y coordinate of the first point.
     * @param  x2 This is the X coordinate of the second point.
     * @param  y2 This is the Y coordinate of the second point.
     * @return    Return the distance.
     */
    float distance_squared(float x1, float y1, float x2, float y2) noexcept;

    /**
     * Calculate the distance between two 2D points without taking square roots. If the square root isn't necessary, then this is faster.
     * @param  a This is the first point.
     * @param  b This is the second point.
     * @return   Return the distance squared.
     */
    float distance_squared(const Point2D &a, const Point2D &b) noexcept;

    /**
     * Calculate the distance between two 3D points without taking square roots. If the square root isn't necessary, then this is faster.
     * @param  x1 This is the X coordinate of the first point.
     * @param  y1 This is the Y coordinate of the first point.
     * @param  z1 This is the Z coordinate of the first point.
     * @param  x2 This is the X coordinate of the second point.
     * @param  y2 This is the Y coordinate of the second point.
     * @param  z2 This is the Z coordinate of the second point.
     * @return    Return the distance squared.
     */
    float distance_squared(float x1, float y1, float z1, float x2, float y2, float z2) noexcept;

    /**
     * Calculate the distance between two 3D points without taking square roots. If the square root isn't necessary, then this is faster.
     * @param  a This is the first point.
     * @param  b This is the second point.
     * @return   Return the distance squared.
     */
    float distance_squared(const Point3D &a, const Point3D &b) noexcept;

    /**
     * Calculate the distance between two 2D points.
     * @param  x1 This is the X coordinate of the first point.
     * @param  y1 This is the Y coordinate of the first point.
     * @param  x2 This is the X coordinate of the second point.
     * @param  y2 This is the Y coordinate of the second point.
     * @return    Return the distance.
     */
    float distance(float x1, float y1, float x2, float y2) noexcept;

    /**
     * Calculate the distance between two 2D points.
     * @param  a This is the first point.
     * @param  b This is the second point.
     * @return   Return the distance.
     */
    float distance(const Point2D &a, const Point2D &b) noexcept;

    /**
     * Calculate the distance between two 3D points.
     * @param  x1 This is the X coordinate of the first point.
     * @param  y1 This is the Y coordinate of the first point.
     * @param  z1 This is the Z coordinate of the first point.
     * @param  x2 This is the X coordinate of the second point.
     * @param  y2 This is the Y coordinate of the second point.
     * @param  z2 This is the Z coordinate of the second point.
     * @return    Return the distance.
     */
    float distance(float x1, float y1, float z1, float x2, float y2, float z2) noexcept;

    /**
     * Calculate the distance between two 3D points.
     * @param  a This is the first point.
     * @param  b This is the second point.
     * @return   Return the distance.
     */
    float distance(const Point3D &a, const Point3D &b) noexcept;

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

        /**
         * Generic curves
         */
        static QuadraticBezier flat() noexcept;
        static QuadraticBezier linear() noexcept;
        static QuadraticBezier ease_in() noexcept;
        static QuadraticBezier ease_out() noexcept;
        static QuadraticBezier ease_in_out() noexcept;
        
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
