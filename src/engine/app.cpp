//===--------------------------------------------------------------------------------------------===
// app.cpp - description
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <glue/app.hpp>
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

namespace amyinorbit::gl {
    App::App(const Window::Attrib& main_window) {
        if(!glfwInit()) throw std::runtime_error("error initialising glfw");
        glfwSetErrorCallback([](int code, const char* message) {
            std::cerr << "gl error (" << code << "): " << message << "\n";
        });

        window_ = Window(main_window);
        window_.make_current();
        gladLoadGL();
        glfwSwapInterval(1);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        last_update_ = glfwGetTime();
        time_.total = 0;
        time_.delta = 0;

        size_.logical = main_window.size;
        size_.physical = window_.framebuffer_size();;
        glViewport(0, 0, size_.physical.x, size_.physical.y);

        // Initialize ImGUI
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::GetIO();

        ImGui_ImplGlfw_InitForOpenGL(window_.impl(), true);
        ImGui_ImplOpenGL3_Init();
        ImGui::StyleColorsDark();
    }

    App::~App() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        show(nullptr);
        window_.destroy();
        glfwTerminate();
    }

    void App::show(Scene* sc) {
        if(scene_) scene_->on_end(*this);
        scene_.reset(sc);
        if(scene_) scene_->on_start(*this);
    }


    void App::run() {
        while(!window_.should_close()) {
            update();
        }
    }

    void App::update() {
        double now = glfwGetTime();
        time_.delta = now - last_update_;
        time_.total += time_.delta;
        last_update_ = now;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if(scene_) {
            scene_->gui(*this);
            scene_->update(*this);
            scene_->render(*this);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window_.swap();
        glfwPollEvents();
    }
}
