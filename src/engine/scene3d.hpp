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
#include "components.hpp"
#include "assets_lib.hpp"
#include <string>
#include <vector>

namespace amyinorbit {
    using namespace amyinorbit::gl;
    using apm::vec3;
    using apm::vec4;
    using apm::int2;

    class Scene3D : public App::Scene {
    public:

        static constexpr int fx_texture_color = 0;
        static constexpr int fx_texture_depth = 1;
        static constexpr int fx_texture_custom = 2;

        Scene3D(App& app, AssetsLib& assets);
        ~Scene3D();

        void on_start(App& app) override {}
        virtual void render_scene(App& app, const RenderData& data) {}
        void render(App& app) override;

    protected:

        AssetsLib& assets() { return assets_; }
        Camera& camera() { return camera_; }
        Light& light() { return light_; }
        vec4& background() { return background_; }

        void set_effects(Shader shader) { quad_shader_ = shader; }
        virtual void prepare_effects(const RenderData& data, Shader& shader) {}

    private:

        mat4 projection() {
            return apm::perspective(apm::radians(camera_.fov), 1024.f/600.f, 0.1f, 1000.f);
        }

        mat4 view() {
            return apm::look_at(camera_.position, camera_.target, vec3(0, 1, 0));
        }

        AssetsLib& assets_;
        Light light_;
        Camera camera_{vec3{0.f, 10.f, 10.f}, vec3{0.f, 0.f, 0.f}, 60.f};

        vec4 background_;

        Framebuffer fbo_;
        Tex2D color_;
        VertexArray quad_vao_;
        Buffer quad_vbo_;
        Shader quad_shader_;
    };
}
