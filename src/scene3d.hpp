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
#include "entity.hpp"
#include <vector>

namespace amyinorbit {
    using namespace amyinorbit::gl;

    class Scene3D : public App::Scene {
    public:
        Scene3D();
        void update(App& app) override;
        void render(App& app) override;
    private:

        std::vector<Entity> entities_;
        mat4 view_;
        mat4 proj_;
    };
}
