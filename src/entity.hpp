//===--------------------------------------------------------------------------------------------===
// object.hpp - description
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <glue/glue.hpp>
#include <apmath/matrix.hpp>

namespace amyinorbit::gl {
    using math::mat4;

    struct Entity;

    struct Component {
        enum Type { graphics, script, max_components };
        virtual ~Component() {}
        virtual void update(Entity& entity) = 0;
    };

    struct Entity {
        mat4 transform = mat4::identity();
        Component* components[Component::max_components] = {nullptr};


        inline Component* operator[](std::size_t index) {
            return components[index];
        }

        inline const Component* operator[](std::size_t index) const {
            return components[index];
        }
    };
};
