//===--------------------------------------------------------------------------------------------===
// trasnform.cpp - Implementation of 3D transformations
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <glue/transform.hpp>

namespace amyinorbit::math {
    mat4 perspective(float fov, float aspect, float near, float far) {

        const float tanHalfFov = std::tan(fov / 2.f);
        // From GLM
        mat4 result(0.f);
        result[0][0] = 1.f / (aspect * tanHalfFov);
    	result[1][1] = 1.f / (tanHalfFov);
    	result[2][2] = - (far + near) / (far - near);
    	result[2][3] = - 1.f;
    	result[3][2] = - (2.f * far * near) / (far - near);
        return result;
    }

    mat4 orthographic(float width, float height, float near, float far) {
        auto zDiff = far - near;
        auto zSum = far + near;

        mat4 result(1.f);
        result[0][0] =  2.f / width;
        result[1][1] = -2.f / height;
        result[2][2] =  2.f / zDiff;
        result[3][0] = -1.f;
        result[3][1] =  1.f;
        result[3][2] = -zSum/zDiff;
        return result;
    }

    mat4 look_at(const float3& camera, const float3& target, const float3& up) {
        auto z = normalize(camera - target);
        auto x = normalize(cross(up, z));
        auto y = normalize(cross(z, x));

        mat4 view;
        view[0] = float4(x.x, y.x, z.x, 0);
        view[1] = float4(x.y, y.y, z.y, 0);
        view[2] = float4(x.z, y.z, z.z, 0);
        view[3] = float4(-dot(x, camera), -dot(y, camera), -dot(z, camera), 1);
        return view;
    }

    mat4 translate(float dx, float dy, float dz) {
      return mat4{
          1.f, 0.f, 0.f, 0.f,
          0.f, 1.f, 0.f, 0.f,
          0.f, 0.f, 1.f, 0.f,
          dx,  dy,  dz,  1.f,
      };
    }

    mat4 rotate(const quaternion& rot) {
        auto r = normalize(rot);

        auto i2 = r.q.x*r.q.x;
        auto j2 = r.q.y*r.q.y;
        auto k2 = r.q.z*r.q.z;

        auto ij = r.q.x*r.q.y;
        auto ik = r.q.x*r.q.z;
        auto jk = r.q.y*r.q.z;

        auto ir = r.w*r.q.x;
        auto jr = r.w*r.q.y;
        auto kr = r.w*r.q.z;

        return mat4{
            1 - 2*(j2 + k2),    2*(ij + kr),    2*(ik - jr),    0,
            2*(ij - kr),        1 - 2*(i2 + k2),2*(jk + ir),    0,
            2*(ik + jr),        2*(jk - ir),    1 - 2*(i2 + j2),0,
            0,                  0,              0,              1,
        };
    }

    mat4 rotate_x(float rx) {
        auto c = std::cos(rx);
        auto s = std::sin(rx);

        return mat4 {
            1.f, 0.f, 0.f, 0.f,
            0.f,  c,  -s,  0.f,
            0.f,  s,   c,  0.f,
            0.f, 0.f, 0.f, 1.f,
        };
    }

    mat4 rotate_y(float ry) {
        auto c = std::cos(ry);
        auto s = std::sin(ry);
        return mat4 {
             c,   0.f, -s, 0.f,
            0.f, 1.f, 0.f, 0.f,
             s,   0.f,  c, 0.f,
            0.f, 0.f, 0.f, 1.f
        };
    }

    mat4 rotate_z(float rz) {
        auto c = std::cos(rz);
        auto s = std::sin(rz);
        return mat4 {
             c,   s,  0.f, 0.f,
            -s,   c,  0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f
        };
    }

    mat4 scale(float sx, float sy, float sz) {
        return mat4{
            sx,  0.f, 0.f, 0.f,
            0.f, sy,  0.f, 0.f,
            0.f, 0.f, sz,  0.f,
            0.f, 0.f, 0.f, 1.f,
        };
    }
}
