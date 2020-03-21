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
    Scene3D::Scene3D(AssetsLib& assets) : assets_(assets) {

        auto test = entities_.create();

        entities_.add_component<int>(test, 123);


        camera_.position = float3(0, 0, 10);
        camera_.target = float3(0, 0, 0);
        camera_.fov = 60.f;

        light_.position = float3(0, 2, 2);
        light_.color = float3(1);

        fbo_ = Framebuffer::create();
        fbo_.bind();

        uint2 size = {1024, 600};

        auto depth = Renderbuffer::create();
        depth.own();
        depth.allocate(TexFormat::depth24_stencil8, size);
        fbo_.attach_depth_stencil(std::move(depth));

        color_ = Tex2D::create();
        color_.own();
        color_.allocate(Tex2D::DataDescr<std::uint8_t>{
            .source_format = TexFormat::rgb,
            .dest_format = TexFormat::rgb,
            .size = size
        });
        fbo_.attach_color(0, color_);
        Framebuffer::unbind_all();
    }

    void Scene3D::update(App &app) {

    }

    std::pair<int, Entity&> Scene3D::create(const Entity::Descr& descr) {

    }

    void Scene3D::render(App &app) {
        fbo_.bind();
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(auto e: entities_.with<Transform>()) {
            e.get_component<Transform>().set_position(float3(1, 0, 0));
        }

        Framebuffer::unbind_all();
    }
}
