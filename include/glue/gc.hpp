//===--------------------------------------------------------------------------------------------===
// gc.hpp - garbage collector template for OpenGL objects
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <type_traits>
#include <unordered_map>
#include <cassert>
#include <string>

// #define GL_GC_TRACE
#ifdef GL_GC_TRACE
#include <iostream>
#endif

namespace amyinorbit::gl {

    template <typename F> struct fn;

    template <typename Ret, typename... Args>
    struct fn<Ret(Args...)> {
        using type = Ret (*)(Args...);
    };

    using GLCreateSingle = fn<GLuint(void)>::type;
    using GLCreateMult= fn<void(GLsizei, GLuint*)>::type;
    using GLDeleteSingle = fn<void(GLuint)>::type;
    using GLDeleteMult = fn<void(GLsizei, GLuint*)>::type;

    namespace detail {

        template <typename Derived>
        class Collector {
        public:
            using Handle = GLuint;

            Collector(const std::string& s) {
                name_ = s;
                #ifdef GL_GC_TRACE
                std::cout << "new collector: " << name_ << "\n";
                #endif
            }

            Handle intern(Handle h) {
                if(!h) return h;
                #ifdef GL_GC_TRACE
                trace(h, "tracked");
                #endif
                refs_[h] = 1;
                return h;
            }

            Handle copy(Handle h) {
                if(!h) return h;
                #ifdef GL_GC_TRACE
                trace(h, "retained");
                #endif
                refs_[h] +=1 ;
                return h;
            }

            void release(Handle h) {
                if(!h) return;
                #ifdef GL_GC_TRACE
                trace(h, "released");
                #endif
                assert(refs_.count(h));
                if(--refs_[h] == 0) {
                    derived().destroy(h);
                    refs_.erase(h);
                    #ifdef GL_GC_TRACE
                    trace(h, "destroyed (" + std::to_string(refs_.size()) + ")");
                    #endif
                }
            }

        private:
            #ifdef GL_GC_TRACE
            void trace(Handle h, const std::string& phase) {
                std::cout << name_ << " ::" << h << " " << phase << "\n";
            }
            #endif

            Derived& derived() { return static_cast<Derived&>(*this); }

            using RefCount = std::uint64_t;
            std::unordered_map<Handle, RefCount> refs_;
            std::string name_;
        };
    }


    class Collector2 : public detail::Collector<Collector2> {
    public:
        using GC = detail::Collector<Collector2>;
        using Constructor = fn<void(GLsizei, GLuint*)>::type;
        using Destructor = fn<void(GLsizei, const GLuint*)>::type;
        using Handle = GC::Handle;

        Collector2(const std::string& name) : Collector(name) { }

        Handle create(Constructor c, Destructor d) {
            Handle h;
            c(1, &h);
            return intern(h, d);
        }

        Handle intern(Handle h, Destructor d) {
            des_ = d;
            return GC::intern(h);
        }

        void destroy(Handle h) {
            des_(1, &h);
        }

    private:
        Destructor des_;
    };

    class Collector1 : public detail::Collector<Collector1> {
    public:
        using GC = detail::Collector<Collector1>;
        using Destructor = fn<void(GLuint)>::type;
        using Handle = GC::Handle;

        Collector1(const std::string& name) : Collector(name) { }

        Handle intern(Handle h, Destructor d) {
            des_ = d;
            return GC::intern(h);
        }

        void destroy(Handle h) {
            des_(h);
        }

    private:
        Destructor des_;
    };

    template <typename T>
    Collector1& gc1() {
        static Collector1 c(typeid(T).name());
        return c;
    }

    template <typename T>
    Collector2& gc2() {
        static Collector2 c(typeid(T).name());
        return c;
    }
}
