//===--------------------------------------------------------------------------------------------===
// assets_lib.hpp - Assets manager (BASCIALLY A BIG OLD CACHE)
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2020 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <glue/glue.hpp>
#include <string>
#include <unordered_map>
#include "obj_loader.hpp"
#include <fstream>
#include <stdexcept>

namespace amyinorbit::gl {

    class AssetsLib {
    public:
        AssetsLib(const std::string& root) : root_(root) {}

        Tex2D texture(const std::string& path) {
            Image img(root_ + "/textures/" + path);
            if(!img.is_loaded()) throw std::runtime_error("cannot open texture file " + path);
            auto tex = Tex2D::create();
            tex.bind();
            tex.upload_data(img);
            tex.gen_mipmaps();
            tex.set_mag_filter(Tex2D::Filter::linear);
            tex.set_min_filter(Tex2D::Filter::nearest);
            tex.set_wrap(Tex2D::Wrap::clamp_edge, Tex2D::Wrap::clamp_edge);
            return tex;
        }

        Shader shader(const std::string& path, Shader::Type type) {
            auto shader = Shader::create(type);
            std::ifstream source(root_ + "/shaders/" + path);
            if(!source.is_open()) throw std::runtime_error("cannot open texture file " + path);
            if(!shader.compile(source)) throw std::runtime_error("error in shader: " + shader.debug_message());
            return shader;
        }

        Mesh model(const std::string& path) {
            std::ifstream file(root_ + "/models/" + path);
            if(!file.is_open()) throw std::runtime_error("cannot open model file " + path);
            return load_object(file);
        }
    private:
        const std::string root_;
    };
}
