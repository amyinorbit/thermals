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
        camera_.position = float3(0, 0, 10);
        camera_.target = float3(0, 0, 0);
        camera_.fov = 60.f;

        light_.position = float3(0, 2, 2);
        light_.color = float3(1);

        Entity::Descr desc;

        desc.vertex_shader = "model.vsh";
        desc.fragment_shader = "model.fsh";
        desc.model = "monkey.obj";

        // create(desc);

        for(int i = 0; i < 10; ++i) {

            auto p = create(desc);
            auto& e = p.second;

            if(i == 4) desc.model = "planet.obj";

            e.transform.translate(float3(i*(0.2*i) - 10.f, 0, 0));
            e.transform.scale(float3(float(i) / 8.f));
            e.transform.set_rotation(quaternion::rotation(M_PI, float3(0, 1, 0)));

        }

    }

    void Scene3D::update(App &app) {

    }

    std::pair<int, Entity&> Scene3D::create(const Entity::Descr& descr) {

        Entity e;
        // Initialise render data
        e.render.vao = VertexArray::create();
        e.render.vbo = Buffer::create(Buffer::vbo);

        e.render.program = Program::create();
        e.render.program.attach(assets_.shader(descr.vertex_shader, Shader::Type::vertex));
        e.render.program.attach(assets_.shader(descr.fragment_shader, Shader::Type::fragment));
        if(!e.render.program.link())
            throw std::runtime_error("error in link step: " + e.render.program.debug_message());

        auto mesh = assets_.model(descr.model);
        e.render.vertex_count = mesh.count;
        e.render.program.use();
        e.render.vao.bind();
        e.render.vbo.bind();
        e.render.vbo.set_data(mesh.data);

        Program::AttrDescr<float> pos;
        pos.offset = 0;
        pos.count = 3;
        pos.stride = 8;

        Program::AttrDescr<float> norm;
        norm.offset = 3;
        norm.count = 3;
        norm.stride = 8;

        Program::AttrDescr<float> uv;
        uv.offset = 6;
        uv.count = 2;
        uv.stride = 8;

        e.render.program.set_attrib_ptr<float>(0, pos);
        e.render.program.enable_attrib(0);

        e.render.program.set_attrib_ptr<float>(1, norm);
        e.render.program.enable_attrib(1);

        e.render.program.set_attrib_ptr<float>(2, uv);
        e.render.program.enable_attrib(2);

        if(descr.texture.size()) e.render.texture = assets_.texture(descr.texture);

        int id = entities_.size();
        entities_.push_back(std::move(e));
        return {id, std::ref(entities_.back())};
    }

    void Scene3D::render(App &app) {

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // This is really not great cache wise, but more than sufficient for our purposes
        for(auto& entity: entities_) {
            RenderData& r = entity.render;
            auto& shader = r.program;
            r.vao.bind();
            if(r.texture) r.texture.bind();

            shader.use();
            shader.set_uniform("blend", 0.f);
            shader.set_uniform("proj", projection());
            shader.set_uniform("view", view());
            shader.set_uniform("model", entity.transform.transform());
            shader.set_uniform("camera.position", camera_.position);
            shader.set_uniform("camera.target", camera_.target);
            shader.set_uniform("light.position", light_.position);
            shader.set_uniform("light.color", light_.color);

            glDrawArrays(GL_TRIANGLES, 0, r.vertex_count);
        }
    }
}
