//===--------------------------------------------------------------------------------------------===
// view.hpp - Component-limited view of a world of entities_
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <ecs/proxy.hpp>
#include <iterator>

namespace amyinorbit::ecs {

    class World;

    template <typename... Cs>
    class EntityIterator {
    public:
        using iterator_tag = std::input_iterator_tag;
        using value_type = Proxy<Cs...>;
        using reference = Proxy<Cs...>;
        using const_reference = const Proxy<Cs...>;

        EntityIterator() : world_(nullptr), index_(MAX_COMPONENTS) {}
        EntityIterator(World* world, Index index) : world_(world), index_(index) { next(); }

        bool operator==(const EntityIterator& other) const { return index_ == other.index_; }
        bool operator!=(const EntityIterator& other) const { return index_ != other.index_; }

        EntityIterator& operator++();
        reference operator*() const;

    private:
        void next();

        World* world_ = nullptr;
        Index index_ = MAX_COMPONENTS;
    };

    template <typename T>
    struct View {
        T begin() const { return begin_; }
        T end() const { return end_; }
        T begin_, end_;
    };
}
