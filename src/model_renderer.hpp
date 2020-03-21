//===--------------------------------------------------------------------------------------------===
// model_renderer.hpp - 3D model rendering system (works with ECS)
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <ecs/world.hpp>
#include <glue/buffer.hpp>
#include <glue/shaders.hpp>
#include <glue/vertex_array.hpp>
#include <apmath/vector.hpp>
#include <apmath/matrix.hpp>
#include "obj_loader.hpp"
#include "assets_lib.hpp"
#include <cstddef>
#include <string>
#include <unordered_map>

namespace amyinorbit {
    using math::float3;
    using math::matrix4;

    struct Light {
        float3 position;
        float3 color;
    };

    struct Camera {
        float3 position;
        float3 target;
        float fov;
    };

    struct Model {
        std::uint32_t offset;
        std::uint32_t vertices;
        gl::Tex2D texture;
    };

    class ModelRenderer {
    public:

        struct Data {
            Camera camera;
            Light light;
            matrix4 view;
            matrix4 projection;
        };

        ModelRenderer(gl::AssetsLib& assets, ecs::World& world)
            : assets_(assets), ecs(world), offset_(0) {}

        void init();
        Model model(const std::string& path);
        void render(const Data& data);
    private:
        gl::AssetsLib& assets_;
        ecs::World& ecs;
        std::size_t offset_ = 0;

        gl::Buffer vbo_;
        gl::VertexArray vao_;
        gl::Shader shader_;

        std::unordered_map<std::string, Model> models_;
    };
}
