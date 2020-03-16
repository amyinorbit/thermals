//===--------------------------------------------------------------------------------------------===
// obj_loader.cpp - description
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include "obj_loader.hpp"
#include <apfun/maybe.hpp>
#include <apfun/ranges.hpp>
#include <sstream>

namespace amyinorbit::gl {

    template<char delim> class split : public std::string {};

    template <char delim>
    std::istream& operator>>(std::istream& is, split<delim>& output) {
       std::getline(is, output, delim);
       return is;
    }

    template <char delim = '-'>
    auto splitstring(std::istringstream& source) {
        return std::vector<string>(
            std::istream_iterator<split<delim>>(source),
            std::istream_iterator<split<delim>>()
        );
    }

    template <typename T>
    const maybe<T>& operator<<(const maybe<T>& m, const char* message) {
        if(!m) {
            throw ParseError(message);
        }
        return m;
    }

    struct Loader {
        std::vector<float3> vertices;
        std::vector<float3> normals;
        std::vector<float2> uvs;
    };

    template <typename T, typename F>
    struct match_helper {
        T&& value;
        F&& f;
    };

    template <typename T, typename F>
    constexpr match_helper<T,F> of(T&& value, F&& f) {
        return match_helper<T,F>{std::forward<T>(value), std::forward<F>(f)};
    }

    template <typename T, typename U, typename F>
    const maybe<T>& operator|(const maybe<T>& m, const match_helper<U, F>& helper) {
        if(m && *m == helper.value) {
            std::invoke(std::forward<F>(helper.f));
        }
        return m;
    }

    constexpr struct strict_t {} strict;

    template <typename T>
    auto ass(T& target) {
        return [&](const T& value) {
            target = value;
        };
    }

    template <typename T>
    auto insert(std::vector<T>& v) {
        return [&](const T& value) {
            v.push_back(value);
        };
    }

    maybe<int> integer(const std::string& in) {
        try {
            return std::stoi(in);
        } catch(std::invalid_argument& e) {
            return nothing();
        }
    }

    maybe<float> floating(const std::string& in) {
        try {
            return std::stof(in);
        } catch(std::invalid_argument& e) {
            return nothing();
        }
    }

    maybe<std::string> shift(std::string& in, const std::string& delim = " \t") {
        if(in.empty()) return nothing();
        auto token_start = in.find_first_not_of(delim);
        if(token_start == string::npos) return nothing();

        auto token_end = in.find_first_of(delim, token_start);

        if(token_end == string::npos) {
            string token;
            in.erase(0, token_start);
            std::swap(token ,in);
            return token;
        } else {
            auto token = in.substr(token_start, token_end - token_start);
            in.erase(token_start, token_end - token_start);
            return token;
        }
    }

    maybe<int3> indices(std::string in) {
        int3 indices(0);

        std::istringstream stream(in);
        auto elements = splitstring<'/'>(stream);
        
        if(elements.empty() || elements.size() > 3) return nothing();

        integer(elements[0]) >> ass(indices[0]) << "invalid vertex index";
        if(elements.size() < 2) return indices;

        indices[1] = integer(elements[1]).join(0);
        if(elements.size() < 3) return indices;

        indices[2] = integer(elements[2]).join(0);
        return indices - 1; // Wavefront format is 1-indexed
    }

    template <typename T>
    bool in_range(const std::vector<T>& v, int i) {
        return i >= 0 && i < v.size();
    }

    auto vertex(const Loader& loader) {
        return [&](const int3& indices) -> maybe<Vertex> {
            Vertex v;
            if(!in_range(loader.vertices, indices[0])) return nothing();
            v.position = loader.vertices[indices[0]];
            if(in_range(loader.uvs, indices[1])) v.tex = loader.uvs[indices[1]];
            if(in_range(loader.normals, indices[2])) v.normal = loader.normals[indices[2]];
            return v;
        };
    }

    void check(bool expr, const string& message) {
        if(!expr)
            throw ParseError(message);
    }

    Mesh load_object(std::istream& in) {
        Mesh mesh;
        Loader loader;
        std::string line;

        while(std::getline(in, line)) {
            if(line.empty()) continue;
            if(line[0] == '#') continue;

            shift(line)
            | of("v", [&]{
                float3 v;
                (shift(line) >= floating) >> ass(v.x) << "missing vertex x coordinate";
                (shift(line) >= floating) >> ass(v.y);
                (shift(line) >= floating) >> ass(v.z);
                loader.vertices.push_back(v);
            })
            | of("vt", [&]{
                float2 v;
                (shift(line) >= floating) >> ass(v.x) << "missing texture u coordinate";
                (shift(line) >= floating) >> ass(v.y);
                loader.uvs.push_back(v);
            })
            | of("vn", [&]{
                float3 v;
                (shift(line) >= floating) >> ass(v.x) << "missing normal x coordinate";
                (shift(line) >= floating) >> ass(v.y);
                (shift(line) >= floating) >> ass(v.z);
                loader.normals.push_back(v);
            })
            | of("f", [&]{
                for(int i = 0; i < 3; ++i) {
                    (shift(line) >= indices >= vertex(loader)) >> insert(mesh.data) << "missing face vertex";
                    mesh.count += 1;
                }
            });
        }
        return mesh;
    }

}
