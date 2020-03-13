//===--------------------------------------------------------------------------------------------===
// image.hpp - *very* thin layer around stb_image to load texture data from files
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <apmath/vector.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <string>

namespace amyinorbit::gl {
    using namespace math;
    using std::string;

    struct Image {

        Image(const Image&) = delete;
        Image(Image&&) = default;
        Image(const string& path);
        ~Image();

        bool is_loaded() const { return data_ != nullptr; }
        const std::uint8_t* data() const { return data_; }
        const int2& size() const { return size_; }
        int channels() const { return channels_; }

        GLenum format() const;
    private:
        int2 size_ = {0, 0};
        int channels_ = 0;
        std::uint8_t* data_ = nullptr;
    };
}
