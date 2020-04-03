//===--------------------------------------------------------------------------------------------===
// texture.hpp - texture wrapper for OpenGL
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <glue/handle.hpp>
#include <glue/enum_utils.hpp>
#include <glue/image.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <apmath/vector.hpp>

namespace amyinorbit::gl {
    using namespace apm;

    enum class TexFormat {
        red = GL_RED,
        rgb = GL_RGB,
        bgr = GL_BGR,
        rgba = GL_RGBA,
        argb = GL_BGRA,
        alpha = GL_ALPHA,
        depth24_stencil8 = GL_DEPTH24_STENCIL8,
        depth_component = GL_DEPTH_COMPONENT,
        depth_stencil = GL_DEPTH_STENCIL,
    };

    template <int Dim>
    class Texture : public Handle<Texture<Dim>> {
    public:
        using Base = Handle<Texture<Dim>>;
        template <typename T>
        struct DataDescr {
            TexFormat source_format;
            TexFormat dest_format;
            vec<std::uint32_t, Dim> size;
        };

        const char* name() { return "texture"; }

        enum class Wrap {
            repeat = GL_REPEAT,
            mirror = GL_MIRRORED_REPEAT,
            clamp_edge = GL_CLAMP_TO_EDGE,
            clamp_border = GL_CLAMP_TO_BORDER,
        };

        enum class Filter {
            linear = GL_LINEAR,
            nearest = GL_NEAREST,
        };

        static Texture create() {
            Texture tex;
            GLuint id;
            glGenTextures(1, &id);
            tex.reset(id);
            return tex;
        }

        GLenum gl_type() const {
            return tex_enum<Dim>;
        }

        void destroy() {
            GLuint name = Base::id();
            glDeleteTextures(1, &name);
        }

        void bind(int unit = 0) const {
            tex_unit_ = unit;
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(gl_type(), Base::id());
        }

        void unbind() const {
            glBindTexture(gl_type(), 0);
        }

        template <typename T, int N = Dim, std::enable_if_t<N == 2>* = nullptr>
        void allocate(const DataDescr<T>& descr) {
            glTexImage2D(gl_type(), 0,
                         static_cast<GLenum>(descr.dest_format),
                         descr.size.x, descr.size.y, 0,
                         static_cast<GLenum>(descr.source_format),
                         as_enum<T>(), nullptr);
        }

        template <typename T, int N = Dim, std::enable_if_t<N == 2>* = nullptr>
        void upload_data(const DataDescr<T>& descr, const T* data) {
            glTexImage2D(gl_type(), 0,
                         static_cast<GLenum>(descr.dest_format),
                         descr.size.x, descr.size.y, 0,
                         static_cast<GLenum>(descr.source_format),
                         as_enum<T>(), data);
        }

        template <typename T, int N = Dim, std::enable_if_t<N == 3>* = nullptr>
        void upload_data(const DataDescr<T>& descr, const T* data) {
            glTexImage3D(gl_type(), 0,
                         static_cast<GLenum>(descr.dest_format),
                         descr.size.x, descr.size.y, descr.size.z, 0,
                         static_cast<GLenum>(descr.source_format),
                         as_enum<T>(), data);
                         
            auto r = glGetError();
            if(r != GL_NO_ERROR) {
                std::cerr << "error: " << r << "\n";
            }
        }

        void upload_data(const Image& img) {
            if(!img.is_loaded()) {
                std::cerr << "IMAGE NOT LOADED\n";
                return;
            }
            glTexImage2D(gl_type(), 0,
                         img.format(),
                         img.size().x, img.size().y, 0,
                         img.format(),
                         GL_UNSIGNED_BYTE, img.data());
        }

        template <int N = Dim, std::enable_if_t<N == 1>* = nullptr>
        void set_wrap(Wrap s) {
            set(GL_TEXTURE_WRAP_S, static_cast<int>(s));
        }

        template <int N = Dim, std::enable_if_t<N == 2>* = nullptr>
        void set_wrap(Wrap x, Wrap y) {
            set(GL_TEXTURE_WRAP_S, static_cast<int>(x));
            set(GL_TEXTURE_WRAP_T, static_cast<int>(y));
        }

        template <int N = Dim, std::enable_if_t<N == 3>* = nullptr>
        void set_wrap(Wrap s, Wrap t, Wrap r) {
            set(GL_TEXTURE_WRAP_S, static_cast<int>(s));
            set(GL_TEXTURE_WRAP_T, static_cast<int>(t));
            set(GL_TEXTURE_WRAP_R, static_cast<int>(r));
        }
        void set_min_filter(Filter f) { set(GL_TEXTURE_MIN_FILTER, static_cast<int>(f)); }
        void set_mag_filter(Filter f) { set(GL_TEXTURE_MAG_FILTER, static_cast<int>(f)); }
        void gen_mipmaps() { glGenerateMipmap(gl_type()); }

        int tex_unit() const { return tex_unit_;}

    private:

        void set(GLenum property, int value) {
            glTexParameteri(gl_type(), property, value);
        }

        mutable int tex_unit_;
    };

    using Tex2D = Texture<2>;
    using Tex3D = Texture<3>;
}
