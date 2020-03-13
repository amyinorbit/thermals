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
#include <memory>

namespace amyinorbit::gl {
    class app {
    public:

        struct scene {
            virtual ~scene() = default;
            virtual void on_start(app& app) = 0;
            virtual void on_end(app& app) = 0;
            virtual void update(app& app) = 0;
        };

        app(const window::attrib& main_window, scene* sc) {
            if(!glfwInit()) throw std::runtime_error("error initialising glfw");
            glfwSetErrorCallback([](int code, const char* message) {
                std::cerr << "gl error (" << code << "): " << message << "\n";
            });

            window_ = window(main_window);
            window_.make_current();
            gladLoadGL();
            glfwSwapInterval(1);

            show(sc);
            last_update_ = glfwGetTime();
        }

        ~app() {
            show(nullptr);
            window_.destroy();
            glfwTerminate();
        }

        void show(scene* sc) {
            if(scene_) scene_->on_end(*this);
            scene_.reset(sc);
            if(scene_) scene_->on_start(*this);
        }

        template <
            typename T,
            typename... Args,
            std::enable_if_t<std::is_constructible_v<T, Args...>>* = nullptr,
            std::enable_if_t<std::is_base_of_v<scene, T>>* = nullptr
        >
        void show(Args&&... args) {
            show(new T(std::forward<Args>(args)...));
        }

        void run() {
            while(!window_.should_close()) {
                update();
            }
        }

        void update() {
            double now = glfwGetTime();
            delta_ = now - last_update_;
            last_update_ = now;
            if(scene_) scene_->update(*this);
            window_.swap();
            glfwPollEvents();
        }

        double time_step() const { return delta_; }

    private:
        window window_;
        std::unique_ptr<scene> scene_ {nullptr};

        double delta_ = 0;
        double last_update_;
    };

    inline void app_main(const window::attrib& cfg, app::scene* sc) {
        app(cfg, sc).run();
    }

    template <
        typename T,
        typename... Args,
        std::enable_if_t<std::is_constructible_v<T, Args...>>* = nullptr,
        std::enable_if_t<std::is_base_of_v<app::scene, T>>* = nullptr
    >
    void app_main(const window::attrib& cfg, Args&&... args) {
        app(cfg, new T(std::forward<Args>(args)...)).run();
    }
}
