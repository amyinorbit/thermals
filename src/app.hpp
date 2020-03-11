//===--------------------------------------------------------------------------------------------===
// app.hpp - Application bootstrapper
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "window.hpp"
#include <functional>
#include <type_traits>

namespace amyinorbit::gl {
    template <typename F, std::enable_if_t<std::is_invocable_v<F>>* = nullptr>
    class app {
    public:
        app(const window::attrib& main_window, F&& f) : update_(std::forward<F>(f)) {
            if(!glfwInit()) throw std::runtime_error("error initialising glfw");
            glfwSetErrorCallback([](int code, const char* message) {
                std::cerr << "gl error (" << code << "): " << message << "\n";
            });

            window_ = window(main_window);
            window_.make_current();
            gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            glfwSwapInterval(1);
        }

        ~app() {
            window_.destroy();
            glfwTerminate();
        }

        void run() {
            while(!window_.should_close()) {
                update();
            }
        }

        void update() {
            std::invoke(std::forward<F>(update_));
            window_.swap();
            glfwPollEvents();
        }

    private:
        window window_;
        F&& update_;
    };

    template <typename F>
    void app_main(const window::attrib& cfg, F&& update_fn) {
        app<F>(cfg, std::forward<F>(update_fn)).run();
    }
}
