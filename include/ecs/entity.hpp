//===--------------------------------------------------------------------------------------------===
// Entity.hpp - Utilities used by entity management
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <cstdint>

#define MAX_ENTITIES 64
#define MAX_COMPONENTS 16

namespace amyinorbit::ecs {

    using Index = std::uint16_t;
    using TypeID = std::uint8_t;
    using TypeMask = std::uint16_t;

    struct Entity {
        Index id;
        Index version;
    };
}
