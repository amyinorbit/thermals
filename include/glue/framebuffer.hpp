//===--------------------------------------------------------------------------------------------===
// framebuffer.hpp - Frame buffer wrapper
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <glue/handle.hpp>
#include <glue/texture.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utility>
#include <unordered_map>

namespace amyinorbit::gl {

    class Renderbuffer : public Handle {
    public:

    private:

    };

    class Framebuffer  : public Handle {
    public:

        static Framebuffer create() {
            Framebuffer tex;
            GLuint id;
            glGenFramebuffers(1, &id);
            tex.reset(id);
            return tex;
        }

        Framebuffer() = default;
        Framebuffer(Framebuffer&& other) : Handle(std::move(other)) {}
        Framebuffer& operator=(Framebuffer&& other) {
            Handle::operator=(std::move(other));
            return *this;
        }

        ~Framebuffer() {
            if(!is_valid()) return;
            GLuint name = id();
            glDeleteFramebuffers(1, &name);
        }

        void bind() { glBindFramebuffer(GL_FRAMEBUFFER, id()); }
        static void unbind_all() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

        // void attach_color(int idx, const Tex2D& tex);
        // void attach_depth_stencil(const Renderbuffer& tex);
        // void attach_depth_stencil(const Tex2D& tex);

    private:
    };
}
