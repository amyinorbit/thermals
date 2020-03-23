//===--------------------------------------------------------------------------------------------===
// model_renderer.cpp - description
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include "model_renderer.hpp"
#include "components.hpp"

namespace amyinorbit {
    using namespace gl;

    ModelRenderer::ModelRenderer(AssetsLib& assets, ecs::World& world)
    : assets_(assets), ecs(world), offset_(0) {
        std::cout << "[init] 3d model rendering module\n";
        vao_ = VertexArray::create();
        vao_.own().bind();

        vbo_ = Buffer::create(Buffer::array_buffer);
        vbo_.own().bind();
        vbo_.allocate<float>(1024 * 1024);

        shader_ = assets_.shader("model.vsh", "model.fsh");
        shader_.own().bind();
        Shader::AttrDescr<float> position, normal, texcoord;
        position.offset = 0;
        position.count = 3;
        position.stride = 8;

        normal.offset = 3;
        normal.count = 3;
        normal.stride = 8;

        texcoord.offset = 6;
        texcoord.count = 2;
        texcoord.stride = 8;

        shader_.set_attrib_ptr(0, position);
        shader_.set_attrib_ptr(1, normal);
        shader_.set_attrib_ptr(2, texcoord);
        shader_.enable_attrib(0);
        shader_.enable_attrib(1);
        shader_.enable_attrib(2);

        vao_.unbind();
    }

    Model ModelRenderer::model(const std::string &path) {
        if(models_.count(path)) return models_.at(path);

        Model m;
        const auto& mesh = assets_.model(path);
        // vao_.bind();
        vbo_.bind();
        m.offset = offset_;
        m.vertices = vbo_.sub_data(m.offset, mesh.data);
        offset_ += m.vertices;
        return m;
    }

    void ModelRenderer::render(const RenderData& data) {
        vao_.bind();
        shader_.bind();
        shader_.set_uniform("camera.position", data.camera.position);
        shader_.set_uniform("camera.target", data.camera.target);
        shader_.set_uniform("light.position", data.light.position);
        shader_.set_uniform("light.color", data.light.color);

        shader_.set_uniform("proj", data.projection);
        shader_.set_uniform("view", data.view);

        for(const auto& [model, transform]: ecs.with<Model, Transform>()) {
            if(model.texture) model.texture.bind();
            shader_.set_uniform("model", transform.transform());
            glDrawArrays(GL_TRIANGLES, model.offset, model.vertices);
        }
    }
}
