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

namespace amyinorbit {
    using namespace gl;

    RayMarcher::RayMarcher(AssetsLib& assets) {
        vao_ = VertexArray::create();
        vao_.own().bind();

        vbo_ = Buffer::create(Buffer::array_buffer);
        vbo_.own().bind();

        static const float quad[] = {
            -1, 1,
            1, -1,
            1, 1,
            -1, 1,
            -1, -1,
            1, -1,
        };
        vbo_.set_data(quad);

        shader_ = assets.shader("raymarch.vert", "clouds.frag");
        shader_.own().bind();

        Shader::AttrDescr<float> position;
        position.count = 2;
        position.stride = 2;
        shader_.set_attrib_ptr(0, position);
        shader_.enable_attrib(0);
        vao_.unbind();

        noise_ = Noise::noise(grid, apm::vec3(100.f), 2.f);
        noise_.own();

        clouds_ = Noise::noise(apm::uvec2(1024, 1024), apm::vec2(100.f), 0.1f);
        clouds_.own();
    }

    void RayMarcher::render(const RenderData &data) {
        vao_.bind();
        shader_.bind();
        noise_.bind(0);
        clouds_.bind(1);

        shader_.set_uniform("noise", noise_);
        shader_.set_uniform("clouds", clouds_);
        shader_.set_uniform("camera.position", data.camera.position);
        shader_.set_uniform("camera.target", data.camera.target);
        shader_.set_uniform("camera.fov", data.camera.fov);
        shader_.set_uniform("light.position", data.light.position);
        shader_.set_uniform("light.color", data.light.color);
        shader_.set_uniform("resolution", data.resolution);
        shader_.set_uniform("projection", data.projection);
        shader_.set_uniform("view", data.view);
        shader_.set_uniform("time", data.time);

        glDrawArrays(GL_TRIANGLES, 0, 6);

    }
}
