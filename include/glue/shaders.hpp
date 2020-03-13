//===--------------------------------------------------------------------------------------------===
// shaders.hpp - Shader and programme wrappers for C++
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <glue/handle.hpp>
#include <apmath/vector.hpp>
#include <apmath/matrix.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <unordered_map>

namespace amyinorbit::gl {
    using namespace math;
    using std::string;

    class shader : public handle {
    public:
        enum type {
            vertex = GL_VERTEX_SHADER,
            fragment = GL_FRAGMENT_SHADER
        };

        shader(type ty) { reset(glCreateShader((GLenum)ty)); }
        ~shader() { if(is_valid()) glDeleteShader(id()); }

        bool compile(const string& source) {
            const char* ptr = source.c_str();
            glShaderSource(id(), 1, &ptr, 0);
            glCompileShader(id());
            return is_compiled();
        }

        bool compile(std::istream& in) {
            std::string source{
                std::istreambuf_iterator<char>(in),
                std::istreambuf_iterator<char>()
            };

            return compile(source);
        }

        bool is_compiled() const {
            GLint value;
            glGetShaderiv(id(), GL_COMPILE_STATUS, &value);
            return value == GL_TRUE;
        }
    };

    class program : public handle {
    public:
        program() {}
        void create() { reset(glCreateProgram()); }
        ~program() { if(is_valid()) glDeleteProgram(id()); }

        void attach(const shader& sh) { glAttachShader(id(), sh.id()); }

        void bind_attr_loc(std::uint32_t loc, const string& attribute) {
            glBindAttribLocation(id(), loc, attribute.c_str());
        }

        bool link() {
            glLinkProgram(id());
            return get(GL_LINK_STATUS) == GL_TRUE;
        }

        void set_attrib_ptr(int loc, int size, GLenum type, bool norm, std::size_t stride, const void* offset) {
            glVertexAttribPointer(loc, size, type, norm, stride, offset);
        }

        template <typename T>
        void set_uniform(int loc, const T& value);

        template <typename T>
        void set_uniform(const std::string& name, const T& value) {
            set_uniform(get_uniform_loc(name), value);
        }

        void use() {
            glUseProgram(id());
        }

    private:
        template <typename K, typename V>
        using map = std::unordered_map<K, V>;

        int get(GLenum what) const {
            GLint out;
            glGetProgramiv(id(), what, &out);
            return out;
        }

        std::uint32_t get_uniform_loc(const string& name) const {
            auto it = uniforms_.find(name);
            if(it != uniforms_.end()) return it->second;
            auto loc = glGetUniformLocation(id(), name.c_str());
            uniforms_[name] = loc;
            return loc;
        }

        std::uint32_t get_attr_loc(const string& name) const {
            auto it = attributes_.find(name);
            if(it != attributes_.end()) return it->second;
            return (attributes_[name] = glGetAttribLocation(id(), name.c_str()));
        }

        mutable map<string, std::uint32_t> attributes_;
        mutable map<string, std::uint32_t> uniforms_;
    };

    template <>
    void program::set_uniform(int loc, const std::int32_t& value) {
        glUniform1i(loc, value);
    }

    template <>
    void program::set_uniform(int loc, const int2& v) {
        glUniform2iv(loc, 2, v.data);
    }

    template <>
    void program::set_uniform(int loc, const int3& v) {
        glUniform3iv(loc, 3, v.data);
    }

    template <>
    void program::set_uniform(int loc, const int4& v) {
        glUniform4iv(loc, 4, v.data);
    }


    template <>
    void program::set_uniform(int loc, const float& value) {
        glUniform1f(loc, value);
    }

    template <>
    void program::set_uniform(int loc, const float2& v) {
        glUniform2fv(loc, 2, v.data);
    }

    template <>
    void program::set_uniform(int loc, const float3& v) {
        glUniform3fv(loc, 3, v.data);
    }

    template <>
    void program::set_uniform(int loc, const float4& v) {
        glUniform4fv(loc, 4, v.data);
    }

    template <>
    void program::set_uniform(int loc, const mat2& m) {
        glUniformMatrix2fv(loc, 1, false, m.data);
    }

    template <>
    void program::set_uniform(int loc, const mat3& m) {
        glUniformMatrix3fv(loc, 1, false, m.data);
    }

    template <>
    void program::set_uniform(int loc, const mat4& m) {
        glUniformMatrix4fv(loc, 1, false, m.data);
    }
}
