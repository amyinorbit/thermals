//===--------------------------------------------------------------------------------------------===
// worley.hpp - worley noise generator. !! very, very inefficient
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <apmath/vector.hpp>
#include <vector>
#include <random>
#include <cmath>
#include <limits>

namespace amyinorbit {

    template <int D>
    class WorleyNoise {
    public:
        using vec = apm::vec<float, D>;

        WorleyNoise(const vec& size, float freq)
            : freq(freq), engine(std::random_device()()), size(size) {
            int count = int(std::pow(freq, float(D)));
            for(int i = 0; i < count; ++i) {
                points.push_back(random_point());
            }
        }

        float operator()(vec r) {
            float dist = std::numeric_limits<float>::infinity();
            r /= size;
            for(const auto& p: points) {
                auto d = apm::length(p-r) * freq;
                if(d < dist) dist = d;
            }
            return dist;
        }
    private:

        vec random_point() {
            std::uniform_real_distribution<float> dist(0.f, 1.f);
            vec v;
            for(int i = 0; i < D; ++i) {
                v[i] = dist(engine);
            }
            return v;
        }
        float freq;
        std::default_random_engine engine;
        vec size;
        std::vector<vec> points;
    };
}
