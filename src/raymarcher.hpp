//===--------------------------------------------------------------------------------------------===
// raymarcher.hpp - infrastructure for a ray raymarcher
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <glue/glue.hpp>
#include <apmath/vector.hpp>
#include "assets_lib.hpp"
#include "components.hpp"

namespace amyinorbit {
    using apm::uvec2;

    class RayMarcher {
    public:
        RayMarcher(AssetsLib& assets);

        void render(const RenderData& data);
    private:
        gl::VertexArray vao_;
        gl::Buffer vbo_;
        gl::Shader shader_;
    };
}
