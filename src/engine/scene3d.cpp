//===--------------------------------------------------------------------------------------------===
// scene3d.cpp - 3D scene implementation
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include "scene3d.hpp"

namespace amyinorbit {

    Scene3D::Scene3D(App& app, AssetsLib& assets) : assets_(assets) {
        camera_.position = vec3(0, 0, 5);
        camera_.target = vec3(0, 0, 0);
        camera_.fov = 60;

        // light_.position = vec3(0, 0, 0);
        light_.color = vec3(1);

        // Set up a framebuffer to render everything

        Framebuffer::Desc<float> desc;
        desc.color_count = 1;
        desc.color[0].source_format = TexFormat::rgb;
        desc.color[0].dest_format = TexFormat::rgb;
        desc.color[0].size = app.point_size();

        desc.depth.size = app.point_size();
        desc.depth.dest_format = TexFormat::depth_component;
        desc.depth.source_format = TexFormat::depth_component;

        fbo_ = Framebuffer(desc);
        auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(status != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "incomplete framebuffer (" << std::hex << status << std::dec << ")\n";
        }
        fbo_.unbind();

        const vec2 quad[] = {
            vec2(-1, 1), vec2(0, 1),
            vec2(1, -1), vec2(1, 0),
            vec2(1, 1), vec2(1, 1),

            vec2(-1, +1), vec2(0, 1),
            vec2(-1, -1), vec2(0, 0),
            vec2(1, -1), vec2(1, 0),
        };

        // Finally, set up the pipeline we'll use to render the scene's quad
        quad_vao_ = VertexArray(VertexArray::Desc{});
        quad_vao_.bind();

        quad_vbo_ = Buffer(Buffer::array_buffer);
        quad_vbo_.bind();
        quad_vbo_.set_data(quad);

        quad_shader_ = assets_.shader("fsquad.vsh", "fsquad.fsh");
        quad_shader_.bind();

        Shader::AttrDescr<float> position, texcoord;
        position.count = 2;
        position.stride = 4;
        position.offset = 0;

        texcoord.count = 2;
        texcoord.stride = 4;
        texcoord.offset = 2;

        quad_shader_.set_attrib_ptr(0, position);
        quad_shader_.set_attrib_ptr(1, texcoord);
        quad_shader_.enable_attrib(0);
        quad_shader_.enable_attrib(1);
    }

    Scene3D::~Scene3D() {
        assets_.clear();
    }

    void Scene3D::render(App &app) {
        fbo_.bind();
        app.viewport(app.point_size());
        glClearColor(background_.r, background_.g, background_.b, background_.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gl_check();

        RenderData render_data;
        render_data.light = light_;
        render_data.camera = camera_;
        render_data.projection = projection();
        render_data.view = view();
        render_data.resolution = vec2(app.point_size().w, app.point_size().h);
        render_data.time = app.time().total;
        render_scene(app, render_data);

        fbo_.unbind();
        app.viewport(app.pixel_size());
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        quad_vao_.bind();
        quad_shader_.bind();
        fbo_.color_attachment(0).bind(fx_texture_color);
        fbo_.depth_attachment().bind(fx_texture_depth);

        quad_shader_.set_uniform("camera.position", camera_.position);
        quad_shader_.set_uniform("camera.target", camera_.target);
        quad_shader_.set_uniform("camera.fov", camera_.fov);
        quad_shader_.set_uniform("light.position", light_.position);
        quad_shader_.set_uniform("light.color", light_.color);
        quad_shader_.set_uniform("resolution", render_data.resolution);
        quad_shader_.set_uniform("color", fbo_.color_attachment(0));
        quad_shader_.set_uniform("depth", fbo_.depth_attachment());
        quad_shader_.set_uniform("time", app.time().total);
        prepare_effects(render_data, quad_shader_);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}
