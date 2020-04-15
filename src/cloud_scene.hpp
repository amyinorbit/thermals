//===--------------------------------------------------------------------------------------------===
// cloud_scene.hpp - Raymarched volumetric cloud rendering (hopefully)
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <ecs/world.hpp>
#include "engine/scene3d.hpp"
#include "engine/model_renderer.hpp"
#include "engine/raymarcher.hpp"
#include "color.hpp"
#include "imgui/imgui.h"

namespace amyinorbit {
    using ecs::Entity;
    using ecs::World;

    inline vec3 cartesian(float el, float az, float d) {
        return vec3(
            d * std::cos(el) * std::sin(az),
            d * std::sin(el),
            d * std::cos(el) * std::cos(az)
        );
    }

    class CloudScene : public Scene3D {
    public:
        CloudScene(App& app, AssetsLib& assets)
        : Scene3D(app, assets)
        , clouds(assets)
        , models(assets, ecs) {
            camera().fov = 60.f;
            camera().position = vec3(40);
            camera().target = vec3(0);
            light().position = vec3(10, 20, 10);
            background().rgb = color::hsv(200, 0.3, 0.9);

            {
                ground = ecs.create();
                auto& m = ecs.add_component<Model>(ground, models.model("plane.obj"));
                m.texture = assets.texture("tex.png");
                m.texture_blend = 1.f;
                auto& t = ecs.add_component<Transform>(ground);
                t.set_scale(world.size);
            }
            set_effects(assets.shader("clouds.vert", "clouds.frag"));
        }

        ~CloudScene() {
            ecs.destroy(ground);
        }

        void gui(App& app) override {
            ImGui::Begin("Camera Settings");
            ImGui::SliderAngle("azimuth", &azimuth);
            ImGui::SliderAngle("elevation", &elevation, 5.f, 80.f);
            ImGui::SliderFloat("distance", &distance, 1.f, 20.f);
            ImGui::SliderFloat("field of view", &camera().fov, 20.f, 80.f, "%.1f deg");
            camera().position = cartesian(elevation, azimuth, distance);
            ImGui::End();
        }

        void update(App& app) override {
        }

        void render_scene(App& app, const RenderData& rd) override {
            models.render(rd);
        }

        void prepare_effects(const RenderData& data, Shader& shader) override {
            clouds.render(data, shader);
        }
    private:
        World ecs;
        RayMarcher clouds;
        ModelRenderer models;

        Entity ground;

        float elevation = apm::radians(45.f);
        float azimuth = apm::radians(90.f);
        float distance = 10.f;
        struct {
            vec3 size{10, 10, 10};
        } world;
    };
}
