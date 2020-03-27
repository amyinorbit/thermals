//===--------------------------------------------------------------------------------------------===
// input.hpp - really basic input handler
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include "window.hpp"
namespace amyinorbit::gl {

    enum class Key {
        left = GLFW_KEY_LEFT,
        right = GLFW_KEY_RIGHT,
        up = GLFW_KEY_UP,
        down = GLFW_KEY_DOWN,
    };

    class Input {
    public:
        Input(const Window& window) : window_(window) {}

        bool down(Key key) const {
            return glfwGetKey(window_.impl(), static_cast<int>(key)) == GLFW_PRESS;
        }

        bool up(Key key) const {
            return glfwGetKey(window_.impl(), static_cast<int>(key)) == GLFW_RELEASE;
        }

    private:
        const Window& window_;
    };
}
