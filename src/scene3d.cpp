//===--------------------------------------------------------------------------------------------===
// scene3d.cpp - 3D scene implementation
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include "scene3d.hpp"

void diagnose() {
    auto err = glGetError();
    if(err == GL_NO_ERROR) return;
    std::cerr << "gl error [" << err << "]\n";
}

namespace amyinorbit {

    Scene3D::Scene3D(AssetsLib& assets) : assets_(assets), renderer_(assets, ecs) {

        renderer_.init();

        camera_.position = float3(0, 0, 10);
        camera_.target = float3(0, 0, 0);
        camera_.fov = 30.f;

        light_.position = float3(0, 2, 2);
        light_.color = float3(1);

        planet = ecs.create();

        ecs.add_component<Model>(planet, renderer_.model("planet.obj"));
        ecs.add_component<Transform>(planet);

        // Set up a framebuffer to render everything

        fbo_ = Framebuffer::create();
        fbo_.bind();

        uint2 size = {1024, 600};

        auto depth = Renderbuffer::create();
        depth.own().bind();
        depth.allocate(TexFormat::depth24_stencil8, size);
        fbo_.attach_depth_stencil(std::move(depth));

        color_ = Tex2D::create();
        color_.own().bind();
        color_.allocate(Tex2D::DataDescr<std::uint8_t>{
            .source_format = TexFormat::rgb,
            .dest_format = TexFormat::rgb,
            .size = size
        });
        color_.set_mag_filter(Tex2D::Filter::linear);
        color_.set_min_filter(Tex2D::Filter::linear);
        fbo_.attach_color(0, color_);


        diagnose();
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "framebuffer incomplete\n";
        Framebuffer::unbind_all();

        const float2 quad[] = {
            float2(-1, 1), float2(0, 1),
            float2(1, -1), float2(1, 0),
            float2(1, 1), float2(1, 1),

            float2(-1, +1), float2(0, 1),
            float2(-1, -1), float2(0, 0),
            float2(1, -1), float2(1, 0),
        };

        // Finally, set up the pipeline we'll use to render the scene's quad
        quad_vao_ = VertexArray::create();
        quad_vao_.own().bind();

        quad_vbo_ = Buffer::create(Buffer::vbo);
        quad_vbo_.own().bind();
        quad_vbo_.set_data(quad);

        quad_shader_ = assets_.shader("fsquad.vsh", "fsquad.fsh");
        quad_shader_.own();
        quad_shader_.use();

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
        ecs.destroy(planet);
        assets_.clear();
    }

    void Scene3D::update(App &app) {
        auto& t = ecs.get_component<Transform>(planet);
        t.set_position(2.f * float3(std::sin(app.time().total), 0, std::cos(app.time().total)));
        t.set_rotation(quaternion::rotation(app.time().total * -2, float3(1)));
    }

    void Scene3D::render(App &app) {
        fbo_.bind();
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ModelRenderer::Data render_data;
        render_data.light = light_;
        render_data.camera = camera_;
        render_data.projection = projection();
        render_data.view = view();

        renderer_.render(render_data);
        Framebuffer::unbind_all();
        //

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        quad_vao_.bind();
        quad_shader_.use();
        color_.bind(0);

        quad_shader_.set_uniform("camera.position", camera_.position);
        quad_shader_.set_uniform("camera.target", camera_.target);
        quad_shader_.set_uniform("light.position", light_.position);
        quad_shader_.set_uniform("light.color", light_.color);
        quad_shader_.set_uniform("resolution", float2(1024, 600));
        quad_shader_.set_uniform("tex", color_);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}
