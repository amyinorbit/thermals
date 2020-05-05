//===--------------------------------------------------------------------------------------------===
// window.hpp - Thin wrapper around GLFW windows
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <GLFW/glfw3.h>
#include <apmath/vector.hpp>
#include <stdexcept>
#include <string>

#pragma once

namespace amyinorbit::gl {
    using std::string;
    using apm::uvec2;


    class Window {
    public:
        struct Attrib {
            uvec2 size = uvec2(1024, 600);
            string name;
            bool is_fullscreen = false;

            struct {
                int major = 4;
                int minor = 0;
            } version;
        };

        Window() : impl_(nullptr) {}
        Window(const Attrib& cfg) {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, cfg.version.major);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, cfg.version.minor);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            impl_ = glfwCreateWindow(
                cfg.size.x,
                cfg.size.y,
                cfg.name.c_str(),
                cfg.is_fullscreen ? glfwGetPrimaryMonitor() : nullptr,
                nullptr
            );
            if(!impl_) {
                throw std::runtime_error("unable to create new window");
            }
        }
        Window(Window&& other) : impl_(other.impl_) { other.impl_ = nullptr; }
        Window& operator=(Window&& other) {
            if(&other != this) {
                impl_ = other.impl_;
                other.impl_ = nullptr;
            }
            return *this;
        }

        ~Window() { destroy(); }

        bool should_close() const { return glfwWindowShouldClose(impl_); }
        void make_current() { glfwMakeContextCurrent(impl_); }
        void swap() { glfwSwapBuffers(impl_); }
        void destroy() {
            if(!impl_) return;
            glfwDestroyWindow(impl_);
            impl_ = nullptr;
        }

        uvec2 framebuffer_size() const {
            int width, height;
            glfwGetFramebufferSize(impl_, &width, &height);
            return uvec2(width, height);
        }

        GLFWwindow* impl() const { return impl_; }

    private:
        GLFWwindow* impl_ = nullptr;
    };
}
