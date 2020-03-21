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
    using std::string;
    using std::unordered_map;

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

        Shader shader(const std::string& vertex, const std::string& fragment) {
            // auto key = vertex + "+" + fragment;
            // if(shaders_.count(key)) return shaders_.at(key);

            auto vs_source = open(root_ + "/shaders/" + vertex);
            auto fs_source = open(root_ + "/shaders/" + fragment);
            return Shader::create(vs_source, fs_source);

            // shaders_[key] = Shader::create(vs_source, fs_source);
            // shaders_[key].own();
            // return shaders_.at(key);
        }

        const Mesh& model(const std::string& path) {
            auto it = meshes_.find(path);
            if(it != meshes_.end()) {
                return meshes_.at(path);
            }
            std::ifstream file(root_ + "/models/" + path);
            if(!file.is_open()) throw std::runtime_error("cannot open model file " + path);
            return meshes_[path] = load_object(file);
        }

        void clear() {
            shaders_.clear();
            meshes_.clear();
        }

    private:
        std::ifstream open(const std::string path) {
            std::ifstream file(path);
            if(!file.is_open())
                throw std::runtime_error("unable to open file at path '" + path + "'");
            return file;
        }

        const std::string root_;
        unordered_map<string, Shader> shaders_;
        unordered_map<string, Mesh> meshes_;
    };
}
