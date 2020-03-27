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

    class Renderbuffer : public Handle<Renderbuffer> {
    public:
        const char* name() { return "render buffer"; }

        static Renderbuffer create() {
            Renderbuffer rbo;
            GLuint id;
            glGenRenderbuffers(1, &id);
            rbo.reset(id);
            return rbo;
        }

        void destroy() {
            GLuint name = id();
            glDeleteFramebuffers(1, &name);
        }

        void bind() const { glBindRenderbuffer(GL_RENDERBUFFER, id()); }
        void unbind() const { glBindRenderbuffer(GL_RENDERBUFFER, 0); }

        void allocate(TexFormat format, const uvec2& size) {
            glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(format), size.x, size.y);
        }

    };

    class Framebuffer  : public Handle<Framebuffer> {
    public:
        const char* name() { return "frame buffer"; }

        static Framebuffer create() {
            Framebuffer tex;
            GLuint id;
            glGenFramebuffers(1, &id);
            tex.reset(id);
            return tex;
        }

        void destroy() {
            GLuint name = id();
            glDeleteFramebuffers(1, &name);
        }

        void bind() const { glBindFramebuffer(GL_FRAMEBUFFER, id()); }
        void unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

        Tex2D& color_attachment(int idx) { return colors_.at(idx); }
        const Tex2D& color_attachment(int idx) const { return colors_.at(idx); }

        void attach_color(int idx, const Tex2D& tex) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, GL_TEXTURE_2D, tex.id(), 0);
            colors_.insert(std::make_pair(idx, tex));
        }

        void attach_color(int idx, Tex2D&& tex) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, GL_TEXTURE_2D, tex.id(), 0);
            colors_.insert(std::make_pair(idx, std::move(tex)));
        }

        void attach_depth_stencil(const Renderbuffer& rbo) {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo.id());
            depth_ = rbo;
        }

        void attach_depth_stencil(Renderbuffer&& rbo) {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo.id());
            depth_ = std::move(rbo);
        }

        static void clear() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

    private:
        std::unordered_map<int, Tex2D> colors_;
        Renderbuffer depth_;
    };
}
