//===--------------------------------------------------------------------------------------------===
// scene3d.cpp - 3D scene implementation
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include "scene3d.hpp"

namespace amyinorbit {
    Scene3D::Scene3D() {

    }

    void Scene3D::update(App &app) {

    }

    void Scene3D::render(App &app) {

        // This is really not great cache wise, but more than sufficient for our purposes
        // for(auto& entity: entities_) {
        //     if(!entity[Component::graphics]) continue;
        //     auto& c = *entity[Component::graphics];
        // };
    }
}
