//===--------------------------------------------------------------------------------------------===
// error.hpp - OpenGL error checking utils
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <string>
#ifndef NDEBUG
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif

namespace amyinorbit::gl {

    #ifndef NDEBUG
    #define gl_check() check_loc(__FUNCTION__, __LINE__)
    #else
    #define gl_check()
    #endif

    inline void check_loc(const std::string& function, int line) {
        #ifndef NDEBUG
        auto err = glGetError();
        if(err == GL_NO_ERROR) return;
        std::cerr << function << "(...) :" << line << ": OpenGL error // ";
        switch (err) {
            case GL_INVALID_ENUM: std::cerr << "invalid enum"; break;
            case GL_INVALID_VALUE: std::cerr << "invalid value"; break;
            case GL_INVALID_OPERATION: std::cerr << "invalid operation"; break;
            case GL_OUT_OF_MEMORY: std::cerr << "out of memory"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: std::cerr << "invalid FBO operation"; break;
        }
        std::cerr << "\n";
        #ifdef GL_ABORT_ON_ERROR
        abort();
        #endif
        #endif
    }
}
