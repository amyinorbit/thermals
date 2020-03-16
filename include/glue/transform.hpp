//===--------------------------------------------------------------------------------------------===
// transform.hpp - 3D transformations
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <apmath/vector.hpp>
#include <apmath/quaternion.hpp>
#include <apmath/matrix.hpp>

namespace amyinorbit::math {

    template <typename T>
    T radians(T deg) { return static_cast<T>(M_PI) * deg / static_cast<T>(180); }

    template <typename T>
    T degrees(T rad) { return static_cast<T>(180) * rad / static_cast<T>(M_PI); }

    mat4 perspective(float width, float height, float near, float far);
    mat4 look_at(const float3& camera, const float3& target, const float3& up = float3(0, 1, 0));

    mat4 translate(float x, float y, float z);
    inline mat4 translate(const float3& v) { return translate(v.x, v.y, v.z); }

    mat4 rotate(const quaternion& q);
    inline mat4 rotate(const float3& axis, float r) {return rotate(quaternion::rotation(r, axis));}
    mat4 rotate_x(float r);
    mat4 rotate_y(float r);
    mat4 rotate_z(float r);

    mat4 scale(float x, float y, float z);
    inline mat4 scale(const float3& s) { return scale(s.x, s.y, s.z); }
    inline mat4 scale(float s) { return scale(s, s, s); }
}
