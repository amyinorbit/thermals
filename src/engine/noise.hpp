//===--------------------------------------------------------------------------------------------===
// noise.hpp - shorthand routines to generate noise textures
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <glue/glue.hpp>
#include <apmath/math.hpp>
#include <apmath/vector.hpp>
#include "SimplexNoise.h"

namespace amyinorbit {
    class Noise {
    public:
        using u32 = std::uint32_t;

        static inline gl::Tex2D noise(const apm::uvec2& res, const apm::vec2& size, float freq) {
            auto h = size / apm::vec2(res);
            auto tex = gl::Tex2D::create();
            float* data = alloc_data(res);

            SimplexNoise gen(freq); // Judge Gen Ahoy!
            for(u32 i = 0; i < res.w; ++i) {
                for(u32 j = 0; j < res.h; ++j) {
                    auto r = apm::vec2(i,j) * h;
                    auto index = (j*res.w) + i;
                    data[index] = apm::remap(gen.fractal(8, r.x, r.y), -1.f, 1.f, 0.f, 1.f);
                }
            }
            tex.bind();
            gl::Tex2D::DataDescr<float> desc;
            desc.source_format = gl::TexFormat::red;
            desc.dest_format = gl::TexFormat::red;
            desc.size = res;
            tex.upload_data(desc, data);
            tex.set_wrap(gl::Tex2D::Wrap::repeat, gl::Tex2D::Wrap::repeat);
            tex.set_mag_filter(gl::Tex2D::Filter::linear);
            tex.gen_mipmaps();
            delete data;
            return tex;
        }

        static inline gl::Tex3D noise(const apm::uvec3& res, const apm::vec3& size, float freq) {
            auto h = size / apm::vec3(res);
            auto tex = gl::Tex3D::create();
            float* data = alloc_data(res);

            SimplexNoise gen(freq); // Judge Gen Ahoy!
            for(u32 i = 0; i < res.x; ++i) {
                for(u32 j = 0; j < res.y; ++j) {
                    for(u32 k = 0; k < res.z; ++k) {
                        auto r = apm::vec3(i,j,k) * h;
                        auto index = (k*res.x*res.y) + (j*res.x) + i;
                        data[index] = apm::remap(gen.fractal(5, r.x, r.y, r.z),
                            -1.f, 1.f, 0.f, 1.f);
                    }
                }
            }
            tex.bind();
            gl::Tex3D::DataDescr<float> desc;
            desc.source_format = gl::TexFormat::red;
            desc.dest_format = gl::TexFormat::red;
            desc.size = res;
            tex.upload_data(desc, data);
            tex.set_wrap(gl::Tex3D::Wrap::repeat, gl::Tex3D::Wrap::repeat, gl::Tex3D::Wrap::repeat);
            tex.set_mag_filter(gl::Tex3D::Filter::linear);
            tex.gen_mipmaps();
            delete data;
            return tex;
        }

    private:
        template <int C>
        static float* alloc_data(const apm::vec<u32, C>& sv) {
            return new float[size(sv)];
        }

        template <int C>
        static u32 size(const apm::vec<u32, C>& sv) {
            return apm::reduce(sv, [](float a, float b) { return a * b; }, 1);
        }
    };
}
