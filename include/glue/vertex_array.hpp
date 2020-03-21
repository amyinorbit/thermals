//===--------------------------------------------------------------------------------------------===
// vertex_array.hpp - VAO oo wrapper
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <glue/handle.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utility>

namespace amyinorbit::gl {
    class VertexArray : public Handle<VertexArray> {
    public:

        static VertexArray create() {
            VertexArray vao;
            GLuint id;
            glGenVertexArrays(1, &id);
            vao.reset(id);
            return vao;
        }

        void destroy() {
            GLuint name = id();
            glDeleteVertexArrays(1, &name);
        }

        void bind() {
            glBindVertexArray(id());
        }
    };
}
