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
#include <glue/enum_utils.hpp>

namespace amyinorbit::gl {
    using namespace math;
    using std::string;

    class Shader : public Handle {
    public:
        enum Type {
            vertex = GL_VERTEX_SHADER,
            fragment = GL_FRAGMENT_SHADER
        };

        static Shader create(Type type) {
            Shader sh;
            sh.reset(glCreateShader((GLenum)type));
            return sh;
        }

        Shader() = default;
        Shader(Shader&& other) : Handle(std::move(other)) {}
        Shader& operator=(Shader&& other) {
            Handle::operator=(std::move(other));
            return *this;
        }
        ~Shader() { if(is_valid()) glDeleteShader(id()); }

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


        std::string debug_message() const {
            char log[512];
            glGetShaderInfoLog(id(), 512, nullptr, log);
            return std::string(log);
        }

        bool is_compiled() const {
            GLint value;
            glGetShaderiv(id(), GL_COMPILE_STATUS, &value);
            return value == GL_TRUE;
        }

    private:
    };

    class Program : public Handle {
    public:

        template <typename T>
        struct AttrDescr {
            int count;
            std::size_t stride;
            std::size_t offset = 0;
            bool normalize = false;
        };

        static Program create() {
            Program p;
            p.reset(glCreateProgram());
            return p;
        }

        Program() = default;
        Program(Program&& other) : Handle(std::move(other)) {}
        Program& operator=(Program&& other) { Handle::operator=(std::move(other)); return *this; }
        ~Program() { if(is_valid()) glDeleteProgram(id()); }

        void attach(const Shader& sh) { glAttachShader(id(), sh.id()); }

        void bind_attr_loc(std::uint32_t loc, const string& attribute) {
            glBindAttribLocation(id(), loc, attribute.c_str());
        }

        bool link() {
            glLinkProgram(id());
            return get(GL_LINK_STATUS) == GL_TRUE;
        }

        std::string debug_message() const {
            char log[512];
            glGetProgramInfoLog(id(), 512, nullptr, log);
            return std::string(log);
        }

        template <typename T>
        void set_attrib_ptr(int loc, const AttrDescr<T>& descr) {
            glVertexAttribPointer(loc,
                                  descr.count,
                                  as_enum<T>(),
                                  descr.normalize,
                                  sizeof(T) * descr.stride,
                                  (const void*)(sizeof(T) * descr.offset));
        }

        template <typename T>
        void set_attrib_ptr(const std::string& name, const AttrDescr<T>& descr) {
            set_attrib_ptr<T>(get_attr_loc(name), descr);
        }

        void enable_attrib(int loc) {
            glEnableVertexAttribArray(loc);
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
    void Program::set_uniform(int loc, const std::int32_t& value) {
        glUniform1i(loc, value);
    }

    template <>
    void Program::set_uniform(int loc, const int2& v) {
        glUniform2iv(loc, 1, v.data);
    }

    template <>
    void Program::set_uniform(int loc, const int3& v) {
        glUniform3iv(loc, 1, v.data);
    }

    template <>
    void Program::set_uniform(int loc, const int4& v) {
        glUniform4iv(loc, 1, v.data);
    }


    template <>
    void Program::set_uniform(int loc, const float& value) {
        glUniform1f(loc, value);
    }

    template <>
    void Program::set_uniform(int loc, const float2& v) {
        glUniform2fv(loc, 1, v.data);
    }

    template <>
    void Program::set_uniform(int loc, const float3& v) {
        glUniform3fv(loc, 1, v.data);
    }

    template <>
    void Program::set_uniform(int loc, const float4& v) {
        glUniform4fv(loc, 1, v.data);
    }

    template <>
    void Program::set_uniform(int loc, const mat2& m) {
        glUniformMatrix2fv(loc, 1, false, m.data);
    }

    template <>
    void Program::set_uniform(int loc, const mat3& m) {
        glUniformMatrix3fv(loc, 1, false, m.data);
    }

    template <>
    void Program::set_uniform(int loc, const mat4& m) {
        glUniformMatrix4fv(loc, 1, false, m.data);
    }
}
