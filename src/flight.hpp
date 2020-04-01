//===--------------------------------------------------------------------------------------------===
// flight.hpp - approximation of a flight sim physics engine
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <apmath/math.hpp>
#include <apmath/vector.hpp>

namespace amyinorbit {
    class Flight {
    public:
        struct Object {
            apm::vec3 velocity;
            float mass;
        };

        Flight();
        ~Flight();

    private:
    };
}
