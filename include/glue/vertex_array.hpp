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

namespace amyinorbit::gl {
    class vertex_array : public handle {
    public:
        vertex_array() {}

        void create() {
            GLuint id;
            glGenVertexArrays(1, &id);
            reset(id);
        }

        ~vertex_array() {
            if(!is_valid()) return;
            GLuint name = id();
            glDeleteVertexArrays(1, &name);
        }

        void bind() {
            glBindVertexArray(id());
        }
    };
}
