//===--------------------------------------------------------------------------------------------===
// object.hpp - description
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <glue/glue.hpp>
#include <apmath/matrix.hpp>
#include <apmath/quaternion.hpp>
#include <apmath/transform.hpp>
#include <apmath/vector.hpp>

namespace amyinorbit {
    using apm::mat4;
    using apm::uvec2;
    using apm::vec2;
    using apm::vec3;
    using apm::quaternion;

    struct Transform {

        void set_position(const vec3& p) { position_ = p; dirty_ = true; }
        void set_position(float x, float y, float z) { set_position(vec3(x,y,z)); }

        void set_rotation(const quaternion& r) { rotation_ = r; dirty_ = true; }

        void set_scale(const vec3& s) { scale_ = s; dirty_ = true; }
        void set_scale(float x, float y, float z) { set_scale(vec3(x,y,z)); }
        void set_scale(float s) { set_scale(vec3(s)); }

        void translate(const vec3& d) { position_ += d; dirty_ = true; }
        void rotate(const quaternion& q) { rotation_ = q * rotation_; dirty_ = true; }
        void scale(const vec3& s) { scale_ *= s; dirty_ = true; }

        vec3 forward() const {
            return apm::rotate(apm::forward<float>, rotation_);
        }

        vec3 up() const {
            return apm::rotate(apm::up<float>, rotation_);
        }

        vec3 right() const {
            return apm::rotate(apm::right<float>, rotation_);
        }

        const vec3& position() const { return position_; }
        const vec3& scale() const { return scale_; }
        const quaternion& rotation() const { return rotation_; }

        const mat4& transform() const {
            if(dirty_) {
                transform_ = apm::translate(position_) * apm::scale(scale_) * apm::mat(rotation_);
                dirty_ = false;
            }
            return transform_;
        }

    private:
        mutable bool dirty_ = true;
        mutable mat4 transform_;

        vec3 position_{0.f};
        vec3 scale_{1.f};
        quaternion rotation_;
    };

    struct Light {
        vec3 position;
        vec3 color;
    };

    struct Camera {
        vec3 position;
        vec3 target;
        float fov;
    };

    struct RenderData {
        Camera camera;
        Light light;
        vec2 resolution;
        mat4 view;
        mat4 projection;

        float time;
    };
};
