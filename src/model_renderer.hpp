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
#include "components.hpp"
#include <cstddef>
#include <string>
#include <unordered_map>

namespace amyinorbit {
    using math::float3;
    using math::matrix4;

    struct Model {
        std::uint32_t offset;
        std::uint32_t vertices;
        gl::Tex2D texture;
    };

    class ModelRenderer {
    public:
        ModelRenderer(AssetsLib& assets, ecs::World& world);
        Model model(const std::string& path);
        void render(const RenderData& data);
    private:
        AssetsLib& assets_;
        ecs::World& ecs;
        std::size_t offset_ = 0;

        gl::Buffer vbo_;
        gl::VertexArray vao_;
        gl::Shader shader_;

        std::unordered_map<std::string, Model> models_;
    };
}
