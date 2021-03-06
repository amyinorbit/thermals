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
#include "input.hpp"
#include <functional>
#include <type_traits>
#include <memory>

namespace amyinorbit::gl {
    class App {
    public:

        struct Time {
            float delta;
            float total;
        };

        struct Scene {
            virtual ~Scene() = default;
            virtual void on_start(App& app) {}
            virtual void on_end(App& app) {}
            virtual void update(App& app) {}
            virtual void render(App& app) {}
            virtual void gui(App& app) {}
        };

        App(const Window::Attrib& main_window);
        ~App();

        void show(Scene* sc);
        void run();
        void update();

        template <
            typename T,
            typename... Args,
            std::enable_if_t<std::is_constructible_v<T, App&, Args...>>* = nullptr,
            std::enable_if_t<std::is_base_of_v<Scene, T>>* = nullptr
        >
        void show(Args&&... args) {
            show(new T(*this, std::forward<Args>(args)...));
        }

        void viewport(const uvec2& size) {
            glViewport(0, 0, size.w, size.h);
        }

        const uvec2& point_size() const { return size_.logical; }
        const uvec2& pixel_size() const { return size_.physical; }

        const Time& time() const { return time_; }
        Input input() const { return Input(window_); }

        Window& window() { return window_; }
        const Window& window() const { return window_; }

    private:
        Window window_;
        std::unique_ptr<Scene> scene_ {nullptr};
        Time time_;
        double last_update_;

        struct {
            apm::uvec2 physical, logical;
        } size_;
    };

    template <
        typename T,
        typename... Args,
        std::enable_if_t<std::is_constructible_v<T, App&, Args...>>* = nullptr,
        std::enable_if_t<std::is_base_of_v<App::Scene, T>>* = nullptr
    >
    void app_main(const Window::Attrib& cfg, Args&&... args) {
        App app(cfg);
        app.show<T>(std::forward<Args>(args)...);
        app.run();
    }
}
