//===--------------------------------------------------------------------------------------------===
// enum_utils.hpp - Utilities to convert from types to GLenum
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdint>

namespace amyinorbit::gl {
    template <typename T> static constexpr GLenum as_enum();

    template <> constexpr GLenum as_enum<float>() { return GL_FLOAT; }
    template <> constexpr GLenum as_enum<int>() { return GL_INT; }
    template <> constexpr GLenum as_enum<unsigned int>() { return GL_UNSIGNED_INT; }
    template <> constexpr GLenum as_enum<double>() { return GL_DOUBLE; }
    template <> constexpr GLenum as_enum<std::uint8_t>() { return GL_UNSIGNED_BYTE; }

    template <int D> static constexpr GLenum tex_enum = GL_NONE;
    template <> static constexpr GLenum tex_enum<1> = GL_TEXTURE_1D;
    template <> static constexpr GLenum tex_enum<2> = GL_TEXTURE_2D;
    template <> static constexpr GLenum tex_enum<3> = GL_TEXTURE_3D;
}
