//===--------------------------------------------------------------------------------------------===
// buffer.hpp - general buffer (EBO/VBO etc) wrapper
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
#include <iostream>
#include <vector>
#include <glue/gc.hpp>

namespace amyinorbit::gl {

    enum class Usage {
        static_draw = GL_STATIC_DRAW,
        dynamic_draw = GL_DYNAMIC_DRAW,
        stream_draw = GL_STREAM_DRAW,
    };

    class Buffer : public Handle<Buffer, 2> {
    public:

        enum Type {
            array_buffer = 0,
            element_buffer = 1,
            uniform_buffer = 2
        };

        int type_id() const {
            return static_cast<int>(type_);
        }

        GLenum gl_type() const {
            static const GLenum mappings[] = {
                [Type::array_buffer] = GL_ARRAY_BUFFER,
                [Type::element_buffer] = GL_ELEMENT_ARRAY_BUFFER,
                [Type::uniform_buffer] = GL_UNIFORM_BUFFER
            };
            return mappings[type_];
        }

        Buffer() {}
        Buffer(Type type) : type_(type) {
            reset(gc().create(glGenBuffers, glDeleteBuffers));
            gl_check();
        }

        void bind() const { glBindBuffer(gl_type(), id()); }
        void unbind() const { glBindBuffer(gl_type(), 0); }

        template <typename T>
        void allocate(std::size_t count, Usage usage = Usage::static_draw) {
            glBufferData(gl_type(), count * sizeof(T), nullptr, static_cast<GLenum>(usage));
            gl_check();
        }

        template <typename T, std::size_t N>
        void set_data(const T (&data)[N], Usage usage = Usage::static_draw) {
            glBufferData(gl_type(), N * sizeof(T), (void*)&data[0], static_cast<GLenum>(usage));
            gl_check();
        }

        template <typename T>
        void set_data(const std::vector<T>& data, Usage usage = Usage::static_draw) {
            glBufferData(gl_type(), data.size() * sizeof(T), (void*)data.data(), static_cast<GLenum>(usage));
            gl_check();
        }

        template <typename T, std::size_t N>
        std::size_t sub_data(std::size_t offset, const T (&data)[N]) {
            std::size_t size = N * sizeof(T);
            offset *= sizeof(T);
            glBufferSubData(gl_type(), (GLintptr)offset, size, (void*)&data[0]);
            gl_check();
            return N;
        }

        template <typename T>
        std::size_t sub_data(std::size_t offset, const std::vector<T>& data) {
            std::size_t size = data.size() * sizeof(T);
            offset *= sizeof(T);
            glBufferSubData(gl_type(), (GLintptr)offset, size, (void*)data.data());
            gl_check();
            return data.size();
        }

    private:
        Type type_;
    };
}
