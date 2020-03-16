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
#include "obj_loader.hpp"

namespace amyinorbit::gl {
    using math::mat4;
    using math::float3;
    using math::quaternion;

    struct Entity;

    struct Component {
        virtual ~Component() {}
        virtual void update(Entity& entity) = 0;
    };

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

    struct RenderData {
        Program program;
        Tex2D texture;
        VertexArray vao;
        Buffer vbo;
        int vertex_count;
    };

    struct Entity {
        struct Descr {
            std::string model;
            std::string texture;
            std::string vertex_shader;
            std::string fragment_shader;
        };

        static constexpr int max_components = 16;
        RenderData render;
        Transform transform;
        Component* components[max_components] = {nullptr};
    };
};
