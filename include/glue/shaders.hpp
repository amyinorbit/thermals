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

    class Stage : public Handle<Stage> {
    public:
        enum Type {
            vertex = GL_VERTEX_SHADER,
            fragment = GL_FRAGMENT_SHADER
        };

        const char* name() { return "shader stage"; }

        static Stage create(Type type) {
            Stage sh;
            return sh.reset(glCreateShader((GLenum)type));
        }

        void destroy() {
            glDeleteShader(id());
        }

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

    class Shader : public Handle<Shader> {
    public:

        template <typename T>
        struct AttrDescr {
            int count;
            std::size_t stride;
            std::size_t offset = 0;
            bool normalize = false;
        };

        const char* name() { return "shader program"; }

        static Shader create(std::istream& vertex, std::istream& fragment) {
            Stage vs = Stage::create(Stage::vertex);
            vs.own();
            if(!vs.compile(vertex))
                throw std::runtime_error("Error in vertex shader: " + vs.debug_message());

            Stage fs = Stage::create(Stage::fragment);
            fs.own();
            if(!fs.compile(fragment))
                throw std::runtime_error("Error in fragment shader: " + fs.debug_message());

            Shader shader;
            shader.reset(glCreateProgram());
            shader.attach(vs);
            shader.attach(fs);
            if(!shader.link())
                throw std::runtime_error("Error in shader: " + shader.debug_message());
            return shader;
        }

        static Shader create() {
            Shader p;
            return p.reset(glCreateProgram());
        }

        void destroy() {
            glDeleteProgram(id());
        }

        void attach(const Stage& sh) { glAttachShader(id(), sh.id()); }

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

        template <int N>
        void set_uniform(int loc, const Texture<N>& tex) {
            glUniform1i(loc, tex.tex_unit());
        }

        void bind() const { glUseProgram(id()); }
        void unbind() const { glUseProgram(0); }

        std::uint32_t get_uniform_loc(const string& name) const {
            auto it = uniforms_.find(name);
            if(it != uniforms_.end()) return it->second;
            auto loc = glGetUniformLocation(id(), name.c_str());
            uniforms_[name] = loc;
            return loc;
        }

    private:
        template <typename K, typename V>
        using map = std::unordered_map<K, V>;

        int get(GLenum what) const {
            GLint out;
            glGetProgramiv(id(), what, &out);
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
    }

    template <>
    inline void Shader::set_uniform(int loc, const int2& v) {
        glUniform2iv(loc, 1, v.data);
    }

    template <>
    inline void Shader::set_uniform(int loc, const ivec3& v) {
        glUniform3iv(loc, 1, v.data);
    }

    template <>
    inline void Shader::set_uniform(int loc, const ivec4& v) {
        glUniform4iv(loc, 1, v.data);
    }


    template <>
    inline void Shader::set_uniform(int loc, const float& value) {
        glUniform1f(loc, value);
    }

    template <>
    inline void Shader::set_uniform(int loc, const vec2& v) {
        glUniform2fv(loc, 1, v.data);
    }

    template <>
    inline void Shader::set_uniform(int loc, const vec3& v) {
        glUniform3fv(loc, 1, v.data);
    }

    template <>
    inline void Shader::set_uniform(int loc, const vec4& v) {
        glUniform4fv(loc, 1, v.data);
    }

    template <>
    inline void Shader::set_uniform(int loc, const mat2& m) {
        glUniformMatrix2fv(loc, 1, false, m.data);
    }

    template <>
    inline void Shader::set_uniform(int loc, const mat3& m) {
        glUniformMatrix3fv(loc, 1, false, m.data);
    }

    template <>
    inline void Shader::set_uniform(int loc, const mat4& m) {
        glUniformMatrix4fv(loc, 1, false, m.data);
    }
}
