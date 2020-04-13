//===--------------------------------------------------------------------------------------------===
// raymarcher.hpp - infrastructure for a ray raymarcher
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <glue/glue.hpp>
#include <apmath/vector.hpp>
#include "assets_lib.hpp"
#include "components.hpp"

namespace amyinorbit {
    using apm::uvec2;


    class RayMarcher {
    public:
        static constexpr apm::uvec3 grid = apm::uvec3(64);

        RayMarcher(AssetsLib& assets);
        void render(const RenderData& data, Shader& shader);
    private:
        gl::Tex3D noise_;
        gl::Tex2D clouds_;
    };
}
