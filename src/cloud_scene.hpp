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
#include "scene3d.hpp"
#include "model_renderer.hpp"
#include "raymarcher.hpp"

namespace amyinorbit {
    using ecs::Entity;
    using ecs::World;

    inline vec3 cartesian(const vec3& s) {
        return vec3(
            s[2] * std::cos(s[1]) * std::cos(s[0]),
            s[2] * std::sin(s[1]),
            s[2] * std::cos(s[1]) * std::sin(s[0])
        );
    }

    class CloudScene : public Scene3D {
    public:
        CloudScene(App& app, AssetsLib& assets)
        : Scene3D(app, assets)
        , clouds(assets)
        , models(assets, ecs) {
            camera().fov = 80.f;
            light().position = vec3(0, 2, 3);
            model = ecs.create();
            ecs.add_component<Model>(model, models.model("planet.obj"));
            auto& t = ecs.add_component<Transform>(model);
            t.set_position({.5f, 0.f, 0.f});
        }

        void update(App& app) override {
            auto in = app.input();
            auto az_speed = app.time().delta * apm::radians(90.f);
            auto el_speed = app.time().delta * apm::radians(90.f);

            if(in.down(Key::left)) {
                cam_pos[0] += az_speed;
            } else if(in.down(Key::right)) {
                cam_pos[0] -= az_speed;
            }

            if(in.down(Key::up)) {
                cam_pos[1] += el_speed;
            } else if(in.down(Key::down)) {
                cam_pos[1] -= el_speed;
            }

            cam_pos[1] = apm::clamp(cam_pos[1], -apm::radians(80.f), apm::radians(80.f));
            camera().position = cartesian(cam_pos);
        }

        void render_scene(App& app, const RenderData& rd) override {
            models.render(rd);
            clouds.render(rd);
        }
    private:
        World ecs;
        RayMarcher clouds;
        ModelRenderer models;

        Entity model;
        vec3 cam_pos = {0.f, 0.f, 5.f};
    };
}
