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

static void print_loc(const char* fn, int l) {
    std::cerr << "trace: " << fn << " (" << l << ")\n";
}

#define TRACE() print_loc(__PRETTY_FUNCTION__, __LINE__)

namespace amyinorbit {
    using ecs::Entity;
    using ecs::World;

    inline vec3 cartesian(const vec3& s) {
        return vec3(
            s[2] * std::cos(s[1]) * std::sin(s[0]),
            s[2] * std::sin(s[1]),
            s[2] * std::cos(s[1]) * std::cos(s[0])
        );
    }

    class CloudScene : public Scene3D {
    public:
        CloudScene(App& app, AssetsLib& assets)
        : Scene3D(app, assets)
        , clouds(assets)
        , models(assets, ecs) {
            camera().fov = 60.f;
            light().position = vec3(10, 20, 10);
            background().rgb = color::hsv(200, 0.3, 0.9);
            TRACE();
            {
                paper_plane = ecs.create();
                auto& m = ecs.add_component<Model>(paper_plane, models.model("paper_plane_nowalls.obj"));
                TRACE();
                m.texture_blend = 0.f;
                auto& t = ecs.add_component<Transform>(paper_plane);
                t.set_position(0, 10, 0);
                camera().target = t.position();
            }
            TRACE();
            {
                ground = ecs.create();
                auto& m = ecs.add_component<Model>(ground, models.model("ground.obj"));
                m.texture = assets.texture("tex.png");
                m.texture.own();
                m.texture_blend = 1.f;
                auto& t = ecs.add_component<Transform>(ground);
                t.set_scale(vec3(100, 80, 100));
            }

        }

        ~CloudScene() {
            ecs.destroy(paper_plane);
            ecs.destroy(ground);
        }

        void update(App& app) override {
            auto in = app.input();
            auto& plane = ecs.get_component<Transform>(paper_plane);

            float f_speed = 2 * app.time().delta;

            float pitch_speed = apm::radians(90.f) * app.time().delta;
            float yaw_speed = apm::radians(45.f) * app.time().delta;

            if(in.down(Key::up)) {
                plane.rotate(apm::rotate(plane.right(), -pitch_speed));
            } else if(in.down(Key::down)) {
                plane.rotate(apm::rotate(plane.right(), pitch_speed));
            }
            if(in.down(Key::left)) {
                plane.rotate(apm::rotate(apm::up<float>, yaw_speed));
            } else if(in.down(Key::right)) {
                plane.rotate(apm::rotate(apm::up<float>, -yaw_speed));
            }

            plane.translate(-f_speed * plane.forward());
            camera().position = plane.position() + 0.5 * plane.forward();
            camera().target=  plane.position();
        }

        void render_scene(App& app, const RenderData& rd) override {
            models.render(rd);
            clouds.render(rd);
        }
    private:
        World ecs;
        RayMarcher clouds;
        ModelRenderer models;

        Entity ground, paper_plane;
    };
}
