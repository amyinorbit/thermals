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

        void reset(std::uint32_t id) {
            id_ = id;
            flags_ |= valid_flag;
        }

        void own() {
            if(is_valid()) {
                flags_ |= owned_flag;
            }
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
        void cleanup() {
            if(!is_owned() || !is_valid()) return;
            static_cast<D&>(*this).destroy();
        }

        static constexpr std::uint32_t valid_flag = 1 << 0;
        static constexpr std::uint32_t owned_flag = 1 << 1;

        std::uint32_t id_;
        std::uint32_t flags_;
    };
}
