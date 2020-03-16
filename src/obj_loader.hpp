//===--------------------------------------------------------------------------------------------===
// obj_loader.hpp - load dirt-simple .obj files
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <apmath/vector.hpp>
#include <unordered_map>
#include <string>
#include <variant>
#include <vector>
#include <utility>
#include <stdexcept>

namespace amyinorbit::gl {
    using math::float2;
    using math::float3;
    using math::float4;
    using math::int3;
    using std::string;

    struct Vertex {
        float3 position;
        float3 normal;
        float2 tex;
    };

    struct ParseError : public std::runtime_error {
        using runtime_error::runtime_error;
    };

    struct Mesh {
        int count = 0;
        std::vector<Vertex> data;
    };

    // using Mesh = std::vector<Vertex>;
    using Object = std::unordered_map<std::string, Mesh>;

    Mesh load_object(std::istream& source);
}
