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
#include <array>

namespace amyinorbit::gl {

    class Renderbuffer : public Handle<Renderbuffer, 2> {
    public:

        Renderbuffer() {}
        Renderbuffer(TexFormat format, const uvec2& size) {
            reset(gc().create(glGenRenderbuffers, glDeleteRenderbuffers));
            gl_check();
            bind();
            allocate(format, size);
        }

        void bind() const { glBindRenderbuffer(GL_RENDERBUFFER, id()); }
        void unbind() const { glBindRenderbuffer(GL_RENDERBUFFER, 0); }

    private:
        void allocate(TexFormat format, const uvec2& size) {
            glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(format), size.x, size.y);
            gl_check();
        }
    };

    class Framebuffer : public Handle<Framebuffer, 2> {
    public:

        template <typename T>
        struct Desc {
            std::uint8_t color_count;
            Tex2D::Desc<T> color[4];
            Tex2D::Desc<T> depth;
        };

        Framebuffer() {}
        template <typename T>
        Framebuffer(const Desc<T>& desc) {
            reset(gc().create(glGenFramebuffers, glDeleteFramebuffers));
            bind();
            for(std::uint8_t i = 0; i < desc.color_count; ++i) {
                attach_color(i, desc.color[i]);
            }
            attach_depth(desc.depth);
        }

        void bind() const { glBindFramebuffer(GL_FRAMEBUFFER, id()); gl_check(); }
        void unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); gl_check(); }


        Tex2D& depth_attachment() { return depth_stencil_; }
        const Tex2D& depth_attachment() const { return depth_stencil_; }

        Tex2D& color_attachment(int idx) { return colors_[idx]; }
        const Tex2D& color_attachment(int idx) const { return colors_[idx]; }

        template <typename T>
        void attach_color(int idx, const Tex2D::Desc<T>& desc) {
            colors_[idx] = Tex2D(desc);
            colors_[idx].bind();
            glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0 + idx,
                GL_TEXTURE_2D,
                colors_[idx].id(), 0
            );
            gl_check();
        }

        template <typename T>
        void attach_depth(const Tex2D::Desc<T>& desc) {
            depth_stencil_ = Tex2D(desc);
            depth_stencil_.bind();
            glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_DEPTH_ATTACHMENT,
                GL_TEXTURE_2D,
                depth_stencil_.id(), 0
            );
            gl_check();
        }

        static void clear() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            gl_check();
        }

    private:
        std::array<Tex2D, 4> colors_;
        Tex2D depth_stencil_;
    };
}
