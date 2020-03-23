//===--------------------------------------------------------------------------------------------===
// context.hpp - sort-of abstraction of an AZDO context
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <cstdint>
#include <glue/buffer.hpp>
#include <glue/texture.hpp>
#include <glue/shaders.hpp>
#include <glue/vertex_array.hpp>
#include <glue/framebuffer.hpp>

namespace amyinorbit::gl {

    class Context {
    public:

        void bind(const Shader& shader) {
            if(bound_.shader == shader.id()) return;
            glUseProgram(shader.id());
            bound_.shader = shader.id();
        }

        void bind(const Buffer& buffer) {
            if(bound_.buffers[buffer.type_id()] == buffer.id()) return;
            glBindBuffer(buffer.gl_type(), buffer.id());
            bound_.buffers[buffer.type_id()] = buffer.id();
        }

        void bind(const VertexArray& vao) {
            if(bound_.vertex_array == vao.id()) return;
            glBindVertexArray(vao.id());
            bound_.vertex_array = vao.id();
        }

        template <int D>
        void bind(const Texture<D>& tex) {
            if(bound_.textures[D] == tex.id()) return;
            glBindTexture(tex.gl_type(), tex.id());
            bound_.textures[D] = tex.id();
        }

    private:
        using ID = std::uint32_t;

        struct Bindings {
            ID vertex_array = 0;
            ID buffers[10] = {0};
            ID textures[6];
            ID shader = 0;
        };

        Bindings bound_;
    };

    Context& GL();
}
