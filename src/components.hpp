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
#include <glue/transform.hpp>
#include <apmath/matrix.hpp>
#include <apmath/quaternion.hpp>
#include <apmath/vector.hpp>

namespace amyinorbit {
    using math::mat4;
    using math::uint2;
    using math::float2;
    using math::float3;
    using math::quaternion;

    struct Transform {

        void set_position(const float3& p) { position_ = p; dirty_ = true; }
        void set_rotation(const quaternion& r) { rotation_ = r; dirty_ = true; }
        void set_scale(const float3& s) { scale_ = s; dirty_ = true; }

        void translate(const float3& d) { position_ += d; dirty_ = true; }
        void rotate(const quaternion& q) { rotation_ *= q; dirty_ = true; }
        void scale(const float3& s) { scale_ *= s; dirty_ = true; }

        const float3& position() const { return position_; }
        const float3& scale() const { return scale_; }
        const quaternion& rotation() const { return rotation_; }

        const mat4& transform() const {
            if(dirty_) {
                transform_ = math::translate(position_) * math::scale(scale_) * math::rotate(rotation_);
                dirty_ = false;
            }
            return transform_;
        }

    private:
        mutable bool dirty_ = true;
        mutable mat4 transform_;

        float3 position_{0.f};
        float3 scale_{1.f};
        quaternion rotation_;
    };

    struct Light {
        float3 position;
        float3 color;
    };

    struct Camera {
        float3 position;
        float3 target;
        float fov;
    };

    struct RenderData {
        Camera camera;
        Light light;
        float2 resolution;
        mat4 view;
        mat4 projection;
    };
};
