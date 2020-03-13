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
#include <vector>

namespace amyinorbit::gl {

    enum class draw {
        once = GL_STATIC_DRAW,
        twice = GL_DYNAMIC_DRAW,
        many = GL_STREAM_DRAW
    };

    class Buffer : public Handle {
    public:

        enum Type {
            vbo = GL_ARRAY_BUFFER,
        };

        static Buffer create(Type type) {
            Buffer bo;
            bo.type_ = type;
            GLuint id;
            glGenBuffers(1, &id);
            bo.reset(id);
            return bo;
        }

        Buffer() = default;
        Buffer(Buffer&& other) : Handle(std::move(other)), type_(other.type_) {}
        Buffer& operator=(Buffer&& other) {
            Handle::operator=(std::move(other));
            type_ = other.type_;
            return *this;
        }

        ~Buffer() {
            if(!is_valid()) return;
            GLuint name = id();
            glDeleteBuffers(1, &name);
        }

        void bind() {
            glBindBuffer((GLenum)type_, id());
        }

        template <typename T, std::size_t N>
        void set_data(const T (&data)[N], draw usage = draw::once) {
            glBufferData((GLenum)type_, N * sizeof(T), (void*)&data[0], static_cast<GLenum>(usage));
        }

        template <typename T>
        void set_data(const std::vector<T>& data, draw usage = draw::once) {
            glBufferData((GLenum)type_, data.size() * sizeof(T), (void*)data.data(), static_cast<GLenum>(usage));
        }

    private:
        Type type_;
    };
}
