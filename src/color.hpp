//===--------------------------------------------------------------------------------------------===
// color.hpp - Color utility functions and such
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <apmath/vector.hpp>
#include <apmath/math.hpp>

namespace amyinorbit {
    using color3 = apm::vec3;
    using color4 = apm::vec4;
    namespace color {
        constexpr color3 white(1.f);
        constexpr color3 black(0.f, 0.f, 0.f);
        constexpr color3 red(1.f, 0.f, 0.f);
        constexpr color3 green(0.f, 1.f, 0.f);
        constexpr color3 blue(0.f, 0.f, 1.f);

        constexpr color3 hsv(float h, float s, float v) {
            double hh = 0.0, p = 0.0, q = 0.0, t = 0.0, ff = 0.0;
            long i = 0;
            color3 out;

            if(s <= 0.0) {       // < is bogus, just shuts up warnings
                out.r = v;
                out.g = v;
                out.b = v;
                return out;
            }
            hh = h;
            if(hh >= 360.0) hh = 0.0;
            hh /= 60.0;
            i = (long)hh;
            ff = hh - i;
            p = v * (1.0 - s);
            q = v * (1.0 - (s * ff));
            t = v * (1.0 - (s * (1.0 - ff)));

            switch(i) {
            case 0:
                out.r = v;
                out.g = t;
                out.b = p;
                break;
            case 1:
                out.r = q;
                out.g = v;
                out.b = p;
                break;
            case 2:
                out.r = p;
                out.g = v;
                out.b = t;
                break;

            case 3:
                out.r = p;
                out.g = q;
                out.b = v;
                break;
            case 4:
                out.r = t;
                out.g = p;
                out.b = v;
                break;
            case 5:
            default:
                out.r = v;
                out.g = p;
                out.b = q;
                break;
            }
            return out;
        }
    }
}
