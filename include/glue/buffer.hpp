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

namespace amyinorbit::gl {

    enum class Usage {
        static_draw = GL_STATIC_DRAW,
        dynamic_draw = GL_DYNAMIC_DRAW,
        stream_draw = GL_STREAM_DRAW,
    };

    class Buffer : public Handle<Buffer> {
    public:

        enum Type {
            vbo = GL_ARRAY_BUFFER,
            ebo = GL_ELEMENT_ARRAY_BUFFER
        };

        static Buffer create(Type type) {
            Buffer bo;
            bo.type_ = type;
            GLuint id;
            glGenBuffers(1, &id);
            bo.reset(id);
            return bo;
        }

        const char* name() { return "buffer"; }

        void destroy() {
            GLuint name = id();
            glDeleteBuffers(1, &name);
        }

        void bind() const {
            glBindBuffer((GLenum)type_, id());
        }

        void unbind() const {
            glBindBuffer((GLenum)type_, 0);
        }

        template <typename T>
        void allocate(std::size_t count, Usage usage = Usage::static_draw) {
            glBufferData((GLenum)type_, count * sizeof(T), nullptr, static_cast<GLenum>(usage));
        }

        template <typename T, std::size_t N>
        void set_data(const T (&data)[N], Usage usage = Usage::static_draw) {
            glBufferData((GLenum)type_, N * sizeof(T), (void*)&data[0], static_cast<GLenum>(usage));
        }

        template <typename T>
        void set_data(const std::vector<T>& data, Usage usage = Usage::static_draw) {
            glBufferData((GLenum)type_, data.size() * sizeof(T), (void*)data.data(), static_cast<GLenum>(usage));
        }

        template <typename T, std::size_t N>
        std::size_t sub_data(std::size_t offset, const T (&data)[N]) {
            std::size_t size = N * sizeof(T);
            offset *= sizeof(T);
            glBufferSubData((GLenum)type_, (GLintptr)offset, size, (void*)&data[0]);
            return N;
        }

        template <typename T>
        std::size_t sub_data(std::size_t offset, const std::vector<T>& data) {
            std::size_t size = data.size() * sizeof(T);
            offset *= sizeof(T);
            glBufferSubData((GLenum)type_, (GLintptr)offset, size, (void*)data.data());
            return data.size();
        }

    private:
        Type type_;
    };
}
