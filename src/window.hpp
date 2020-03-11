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
#include <glm/vec2.hpp>
#include <stdexcept>
#include <string>

namespace amyinorbit::gl {
    using std::string;
    using glm::ivec2;


    class window {
    public:
        struct attrib {
            ivec2 size;
            string name;

            struct {
                int major;
                int minor;
            } version;
        };

        window() : impl_(nullptr) {}
        window(const attrib& cfg) {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, cfg.version.major);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, cfg.version.minor);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            impl_ = glfwCreateWindow(cfg.size.x, cfg.size.y, cfg.name.c_str(), nullptr, nullptr);
            if(!impl_) {
                throw std::runtime_error("unable to create new window");
            }
        }
        window(window&& other) : impl_(other.impl_) { other.impl_ = nullptr; }
        window& operator=(window&& other) {
            if(&other != this) {
                impl_ = other.impl_;
                other.impl_ = nullptr;
            }
            return *this;
        }

        ~window() { destroy(); }

        bool should_close() const { return glfwWindowShouldClose(impl_); }
        void make_current() { glfwMakeContextCurrent(impl_); }
        void swap() { glfwSwapBuffers(impl_); }
        void destroy() {
            if(!impl_) return;
            glfwDestroyWindow(impl_);
            impl_ = nullptr;
        }

        ivec2 framebuffer_size() const {
            ivec2 size;
            glfwGetFramebufferSize(impl_, &size.x, &size.y);
            return size;
        }

    private:

        GLFWwindow* impl_ = nullptr;
    };
}