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
#include <glue/gc.hpp>
#include <glue/error.hpp>
namespace amyinorbit::gl {

    template <typename D, int N>
    struct Handle {
        Handle() : id_(0) {}
        explicit Handle(std::uint32_t id) : id_(id) {}
        Handle(const Handle& other) : id_(gc().copy(other.id_)) { }

        Handle& operator=(const Handle& other) {
            gc().release(id_);
            id_ = gc().copy(other.id_);
            return *this;
        }

        virtual ~Handle() {
            gc().release(id_);
        }

        std::uint32_t id() const { return id_; }

        bool is_valid() const { return id_ != 0; }
        operator bool() const { return is_valid(); }
        operator std::uint32_t() const { return id_; }

        bool operator==(const Handle& other) const { return id_ == other.id_; }
        bool operator!=(const Handle& other) const { return !(*this == other); }

        D& reset(std::uint32_t id) {
            id_ = id;
            return derived();
        }

    protected:

        D& derived() {
            return static_cast<D&>(*this);
        }

        template <int NN = N, std::enable_if_t<NN == 1>* = nullptr>
        Collector1& gc() const { return gc1<D>(); }

        template <int NN = N, std::enable_if_t<NN == 2>* = nullptr>
        Collector2& gc() const { return gc2<D>(); }

    private:
        std::uint32_t id_;
    };
}
