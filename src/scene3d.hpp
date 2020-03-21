//===--------------------------------------------------------------------------------------------===
// TestScene.hpp - Testing stuff
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <glue/glue.hpp>
#include <apmath/vector.hpp>
#include <ecs/world.hpp>
#include "components.hpp"
#include "model_renderer.hpp"
#include "assets_lib.hpp"
#include <string>
#include <vector>

namespace amyinorbit {
    using namespace amyinorbit::gl;
    using math::float3;
    using math::int2;

    class Scene3D : public App::Scene {
    public:

        Scene3D(AssetsLib& assets);
        ~Scene3D();
        void update(App& app) override;
        void render(App& app) override;
    private:

        mat4 projection() {
            return math::perspective(math::radians(camera_.fov), 1024.f/600.f, 0.1f, 100.f);
        }

        mat4 view() {
            return math::look_at(camera_.position, camera_.target);
        }

        ecs::World ecs;
        AssetsLib& assets_;
        ModelRenderer renderer_;
        Light light_;
        Camera camera_{float3{0.f, 10.f, 10.f}, float3{0.f, 0.f, 0.f}, 60.f};
        ecs::Entity planet;


        Framebuffer fbo_;
        Tex2D color_;
        VertexArray quad_vao_;
        Buffer quad_vbo_;
        Shader quad_shader_;
    };
}
