//===--------------------------------------------------------------------------------------------===
// raymarcher.cpp - description
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include "raymarcher.hpp"
#include "noise.hpp"
#include "scene3d.hpp"

namespace amyinorbit {
    using namespace gl;

    RayMarcher::RayMarcher(AssetsLib& assets) {
        noise_ = Noise::p_worley(grid, apm::vec3(100.f), 5.f);
        clouds_ = Noise::perlin(apm::uvec2(1024, 1024), apm::vec2(100.f), 0.1f);
    }

    void RayMarcher::render(const RenderData &data, Shader& shader) {
        noise_.bind(Scene3D::fx_texture_custom + 0);
        clouds_.bind(Scene3D::fx_texture_custom + 1);

        shader.set_uniform("noise", noise_);
        shader.set_uniform("clouds", clouds_);
        shader.set_uniform("projection", data.projection);
        shader.set_uniform("view", data.view);
    }
}
