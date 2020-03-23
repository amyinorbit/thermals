//===--------------------------------------------------------------------------------------------===
// handle.hpp - description
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <cstdint>
#include <cstdlib>

#ifndef NDEBUG
#include <iostream>
#endif

namespace amyinorbit::gl {

    template <typename D>
    struct Handle {
        Handle() : id_(0), flags_(0) {}
        explicit Handle(std::uint32_t id) : id_(id), flags_(valid_flag) {}

        Handle(Handle&& other) : id_(other.id_), flags_(other.flags_) {
            other.id_ = 0;
            other.flags_ = 0;
        }

        Handle(const Handle& other) : id_(other.id_), flags_(other.flags_ & (~owned_flag)) { }

        Handle& operator=(Handle&& other) {
            if(&other != this) {
                cleanup();
                id_ = other.id_;
                flags_ = other.flags_;
                other.flags_ = 0;
                other.id_ = 0;
            }
            return *this;
        }

        Handle& operator=(const Handle& other) {
            if(&other != this) {
                cleanup();
                id_ = other.id_;
                flags_ = other.flags_ & (~owned_flag);
            }
            return *this;
        }

        virtual ~Handle() {
            cleanup();
        }

        void bind() const {
            D::bind(id_);
        }

        void unbind() const {
            D::bind(0);
        }

        D& reset(std::uint32_t id) {
            id_ = id;
            flags_ = valid_flag;
            return impl();
        }

        D& own() {
            if(is_valid()) {
                flags_ |= owned_flag;
            }
            #ifndef NDEBUG
            std::cerr << "[" << impl().name() << "]: owning #" << id_ << "\n";
            #endif
            return impl();
        }

        std::uint32_t id() const { return id_; }

        bool is_owned() const { return (flags_ & owned_flag) != 0; }
        bool is_valid() const { return (flags_ & valid_flag) != 0; }
        operator bool() const { return is_valid(); }
        operator std::uint32_t() const { return id_; }

        bool operator==(const Handle& other) const {
            return id_ == other.id_ && flags_ == other.flags_;
        }

        bool operator!=(const Handle& other) const {
            return !(*this == other);
        }

    private:
        D& impl() { return static_cast<D&>(*this); }

        void cleanup() {
            if(!is_owned() || !is_valid()) return;

            #ifndef NDEBUG
            std::cerr << "[" << impl().name() << "]: destroying #" << id_ << "\n";
            #endif
            static_cast<D&>(*this).destroy();
        }

        static constexpr std::uint32_t valid_flag = 1 << 0;
        static constexpr std::uint32_t owned_flag = 1 << 1;

        std::uint32_t id_;
        std::uint32_t flags_;
    };
}
