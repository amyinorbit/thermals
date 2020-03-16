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
#include "entity.hpp"
#include <string>
#include <vector>

namespace amyinorbit {
    using namespace amyinorbit::gl;
    using math::float3;
    using math::int2;

    struct Camera {
        float3 position;
        float3 target;
        float fov;
    };

    class Scene3D : public App::Scene {
    public:
        struct Descr {
            Camera camera;
            int2 size;
        };

        Scene3D();
        void update(App& app) override;
        void render(App& app) override;
    private:
        std::vector<Entity> entities_;

        Camera camera_{float3{0.f, 10.f, 10.f}, float3{0.f, 0.f, 0.f}, 60.f};

    };
}
