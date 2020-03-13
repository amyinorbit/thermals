//===--------------------------------------------------------------------------------------------===
// image.cpp - thin image wrapper
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <glue/image.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace amyinorbit::gl {

    Image::Image(const string& path) {
        stbi_set_flip_vertically_on_load(true);
        data_ = stbi_load(path.c_str(), &size_.x, &size_.y, &channels_, 0);
    }

    Image::~Image() {
        if(data_) stbi_image_free(data_);
    }

    GLenum Image::format() const {
        switch(channels_) {
            case 1: return GL_RED;
            case 2: return GL_RG;
            case 3: return GL_RGB;
            case 4: return GL_RGBA;
            default: break;
        }
        return GL_NONE;
    }
}
