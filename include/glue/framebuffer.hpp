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

        static Renderbuffer create() {
            Renderbuffer rbo;
            GLuint id;
            glGenRenderbuffers(1, &id);
            rbo.reset(id);
            return rbo;
        }

        Renderbuffer() = default;
        Renderbuffer(Renderbuffer&& other) : Handle(std::move(other)) {}
        Renderbuffer& operator=(Renderbuffer&& other) {
            Handle::operator=(std::move(other));
            return *this;
        }

        ~Renderbuffer() {
            if(!is_valid()) return;
            GLuint name = id();
            glDeleteFramebuffers(1, &name);
        }

        void bind() { glBindRenderbuffer(GL_RENDERBUFFER, id()); }
        static void unbind_all() { glBindRenderbuffer(GL_RENDERBUFFER, 0); }

        void allocate(TexFormat format, const uint2& size) {
            glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(format), size.x, size.y);
        }

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
        Framebuffer(Framebuffer&& other)
            : Handle(std::move(other)), colors_(std::move(other.colors_)), depth_(std::move(other.depth_)) {}
        Framebuffer& operator=(Framebuffer&& other) {
            Handle::operator=(std::move(other));
            if(this != &other) {
                colors_ = std::move(other.colors_);
                depth_ = std::move(other.depth_);
            }
            return *this;
        }

        ~Framebuffer() {
            if(!is_valid()) return;
            GLuint name = id();
            glDeleteFramebuffers(1, &name);
        }

        void bind() { glBindFramebuffer(GL_FRAMEBUFFER, id()); }
        static void unbind_all() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

        Tex2D& color_attachment(int idx) { return colors_.at(idx); }
        const Tex2D& color_attachment(int idx) const { return colors_.at(idx); }

        void attach_color(int idx, Tex2D&& tex) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, GL_TEXTURE_2D, tex.id(), 0);
            colors_.insert(std::make_pair(idx, std::move(tex)));
            // colors_[idx] = std::move(tex);
        }

        void attach_depth_stencil(Renderbuffer&& rbo) {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo.id());
            depth_ = std::move(rbo);
        }

    private:
        std::unordered_map<int, Tex2D> colors_;
        Renderbuffer depth_;
    };
}
