//===--------------------------------------------------------------------------------------------===
// Stages.hpp - Stage and Shaderme wrappers for C++
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <glue/handle.hpp>
#include <glue/enum_utils.hpp>
#include <glue/texture.hpp>
#include <apmath/vector.hpp>
#include <apmath/matrix.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <unordered_map>

namespace amyinorbit::gl {
    using namespace apm;
    using std::string;

    class Stage : public Handle<Stage, 1> {
    public:
        enum Type {
            vertex = GL_VERTEX_SHADER,
            fragment = GL_FRAGMENT_SHADER
        };

        Stage() {}
        Stage(Type type) {
            reset(glCreateShader(static_cast<GLenum>(type)));
            gl_check();
            gc().intern(id(), glDeleteShader);
        }

        bool compile(const string& source) {
            const char* ptr = source.c_str();
            glShaderSource(id(), 1, &ptr, 0);
            gl_check();
            glCompileShader(id());
            gl_check();
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

    class Shader : public Handle<Shader, 1> {
    public:

        struct Attr {
            Shader& shader;
            string name;
            int location;
        };

        template <typename T>
        struct AttrDescr {
            int count;
            std::size_t stride;
            std::size_t offset = 0;
            bool normalize = false;
        };

        Shader() {}
        Shader(std::istream& vertex, std::istream& fragment) {
            reset(glCreateProgram());
            gl_check();
            gc().intern(id(), glDeleteProgram);

            Stage vs = Stage(Stage::vertex);
            if(!vs.compile(vertex))
                throw std::runtime_error("Error in vertex shader: " + vs.debug_message());

            Stage fs = Stage(Stage::fragment);
            if(!fs.compile(fragment))
                throw std::runtime_error("Error in fragment shader: " + fs.debug_message());

            attach(vs);
            attach(fs);
            if(!link())
                throw std::runtime_error("Error in shader: " + debug_message());
        }

        void attach(const Stage& sh) { glAttachShader(id(), sh.id()); }

        void bind_attr_loc(std::uint32_t loc, const string& attribute) {
            glBindAttribLocation(id(), loc, attribute.c_str());
            gl_check();
        }

        bool link() {
            glLinkProgram(id());
            gl_check();
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
            gl_check();
        }

        template <typename T>
        void set_attrib_ptr(const std::string& name, const AttrDescr<T>& descr) {
            set_attrib_ptr<T>(get_attr_loc(name), descr);
            gl_check();
        }

        void enable_attrib(int loc) {
            glEnableVertexAttribArray(loc);
            gl_check();
        }

        template <typename T>
        void set_uniform(int loc, const T& value);

        template <typename T>
        void set_uniform(const std::string& name, const T& value) {
            set_uniform(get_uniform_loc(name), value);
        }

        template <int N>
        void set_uniform(int loc, const Texture<N>& tex) {
            glUniform1i(loc, tex.tex_unit());
            gl_check();
        }

        void bind() const { glUseProgram(id()); }
        void unbind() const { glUseProgram(0); }

        std::uint32_t get_uniform_loc(const string& name) const {
            auto it = uniforms_.find(name);
            if(it != uniforms_.end()) return it->second;
            auto loc = glGetUniformLocation(id(), name.c_str());
            gl_check();
            uniforms_[name] = loc;
            return loc;
        }

    private:
        template <typename K, typename V>
        using map = std::unordered_map<K, V>;

        int get(GLenum what) const {
            GLint out;
            glGetProgramiv(id(), what, &out);
            gl_check();
            return out;
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
    inline void Shader::set_uniform(int loc, const std::int32_t& value) {
        glUniform1i(loc, value);
        gl_check();
    }

    template <>
    inline void Shader::set_uniform(int loc, const int2& v) {
        glUniform2iv(loc, 1, v.data);
        gl_check();
    }

    template <>
    inline void Shader::set_uniform(int loc, const ivec3& v) {
        glUniform3iv(loc, 1, v.data);
        gl_check();
    }

    template <>
    inline void Shader::set_uniform(int loc, const ivec4& v) {
        glUniform4iv(loc, 1, v.data);
        gl_check();
    }


    template <>
    inline void Shader::set_uniform(int loc, const float& value) {
        glUniform1f(loc, value);
        gl_check();
    }

    template <>
    inline void Shader::set_uniform(int loc, const vec2& v) {
        glUniform2fv(loc, 1, v.data);
        gl_check();
    }

    template <>
    inline void Shader::set_uniform(int loc, const vec3& v) {
        glUniform3fv(loc, 1, v.data);
        gl_check();
    }

    template <>
    inline void Shader::set_uniform(int loc, const vec4& v) {
        glUniform4fv(loc, 1, v.data);
        gl_check();
    }

    template <>
    inline void Shader::set_uniform(int loc, const mat2& m) {
        glUniformMatrix2fv(loc, 1, false, m.data);
        gl_check();
    }

    template <>
    inline void Shader::set_uniform(int loc, const mat3& m) {
        glUniformMatrix3fv(loc, 1, false, m.data);
        gl_check();
    }

    template <>
    inline void Shader::set_uniform(int loc, const mat4& m) {
        glUniformMatrix4fv(loc, 1, false, m.data);
        gl_check();
    }
}
