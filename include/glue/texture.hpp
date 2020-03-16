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
    using namespace math;

    template <int D> constexpr GLenum tex_enum = GL_NONE;
    template <> constexpr GLenum tex_enum<1> = GL_TEXTURE_1D;
    template <> constexpr GLenum tex_enum<2> = GL_TEXTURE_2D;
    template <> constexpr GLenum tex_enum<3> = GL_TEXTURE_3D;

    enum class TexFormat {
        red = GL_RED,
        rgb = GL_RGB,
        bgr = GL_BGR,
        rgba = GL_RGBA,
        argb = GL_BGRA,
        depth_component = GL_DEPTH_COMPONENT,
        depth_stencil = GL_DEPTH_STENCIL,
    };

    template <int Dim>
    class Texture : public Handle {
    public:
        template <typename T>
        struct DataDescr {
            TexFormat source_format;
            TexFormat dest_format;
            vec<std::uint32_t, Dim> size;
        };

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

        Texture() = default;
        Texture(Texture&& other) : Handle(std::move(other)) {}
        Texture& operator=(Texture&& other) {
            Handle::operator=(std::move(other));
            return *this;
        }

        ~Texture() {
            if(!is_valid()) return;
            GLuint name = id();
            glDeleteTextures(1, &name);
        }

        void bind(int unit = 0) {
            tex_unit_ = unit;
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(tex_enum<Dim>, id());
        }

        template <typename T, int N = Dim, std::enable_if_t<N == 2>* = nullptr>
        void upload_data(const DataDescr<T>& descr, const T* data) {
            glTexImage2D(tex_enum<Dim>, 0,
                         static_cast<GLenum>(descr.dest_format),
                         descr.size.x, descr.size.y, 0,
                         static_cast<GLenum>(descr.source_format),
                         as_enum<T>(), data);
        }

        template <typename T, int N = Dim, std::enable_if_t<N == 3>* = nullptr>
        void upload_data(const DataDescr<T>& descr, const T* data) {
            glTexImage3D(tex_enum<Dim>, 0,
                         static_cast<GLenum>(descr.dest_format),
                         descr.size.x, descr.size.y, descr.size.z, 0,
                         static_cast<GLenum>(descr.source_format),
                         as_enum<T>(), data);
        }

        void upload_data(const Image& img) {
            if(!img.is_loaded()) {
                std::cerr << "IMAGE NOT LOADED\n";
                return;
            }
            glTexImage2D(tex_enum<Dim>, 0,
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

        template <int N = Dim, std::enable_if_t<N == 2>* = nullptr>
        void set_wrap(Wrap s, Wrap t, Wrap r) {
            set(GL_TEXTURE_WRAP_S, static_cast<int>(s));
            set(GL_TEXTURE_WRAP_T, static_cast<int>(t));
            set(GL_TEXTURE_WRAP_R, static_cast<int>(r));
        }
        void set_min_filter(Filter f) { set(GL_TEXTURE_MIN_FILTER, static_cast<int>(f)); }
        void set_mag_filter(Filter f) { set(GL_TEXTURE_MAG_FILTER, static_cast<int>(f)); }
        void gen_mipmaps() { glGenerateMipmap(tex_enum<Dim>); }

        int tex_unit() const { return tex_unit_;}

    private:

        void set(GLenum property, int value) {
            glTexParameteri(tex_enum<Dim>, property, value);
        }

        int tex_unit_;
    };

    using Tex2D = Texture<2>;
    using Tex3D = Texture<3>;
}
