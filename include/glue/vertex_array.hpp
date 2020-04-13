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
#include <glue/enum_utils.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utility>
#include <iostream>

namespace amyinorbit::gl {
    class VertexArray : public Handle<VertexArray, 2> {
    public:
        struct Desc {};

        template <typename T>
        struct AttrDesc {
            int count;
            std::size_t stride;
            std::size_t offset = 0;
            bool normalize = false;
        };

        VertexArray() {}
        VertexArray(const Desc& desc) {
            reset(gc().create(glGenVertexArrays, glDeleteVertexArrays));
            gl_check();
        }

        void bind() const { glBindVertexArray(id()); }
        void unbind() const { glBindVertexArray(0); }
    };
}
