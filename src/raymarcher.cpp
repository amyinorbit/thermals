//===--------------------------------------------------------------------------------------------===
// raymarcher.cpp - description
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include "raymarcher.hpp"

namespace amyinorbit {
    using namespace gl;

    RayMarcher::RayMarcher(AssetsLib& assets) {
        vao_ = VertexArray::create();
        vao_.own().bind();

        vbo_ = Buffer::create(Buffer::vbo);
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

        shader_ = assets.shader("raymarch.vsh", "raymarch.fsh");
        shader_.own().bind();

        Shader::AttrDescr<float> position;
        position.count = 2;
        position.stride = 2;
        shader_.set_attrib_ptr(0, position);
        shader_.enable_attrib(0);
        vao_.unbind();
    }

    void RayMarcher::render(const RenderData &data) {
        vao_.bind();
        shader_.bind();

        shader_.set_uniform("camera.position", data.camera.position);
        shader_.set_uniform("camera.target", data.camera.target);
        shader_.set_uniform("light.position", data.light.position);
        shader_.set_uniform("light.color", data.light.color);
        shader_.set_uniform("resolution", data.resolution);

        glDrawArrays(GL_TRIANGLES, 0, 6);

    }
}
