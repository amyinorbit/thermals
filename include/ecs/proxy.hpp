//===--------------------------------------------------------------------------------------------===
// proxy.hpp - Entity fetch proxy
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <ecs/entity.hpp>
#include <ecs/meta.hpp>
#include <type_traits>

namespace amyinorbit::ecs {
    class World;

    template <typename... Cs>
    struct Proxy {
        Proxy(World* world, Entity entity) : world_(world), entity_(entity) {}

        template <typename C>
        auto get_component() -> std::enable_if_t<in_list_v<C, Cs...>, C&>;

        template <typename C>
        auto get_component() const -> std::enable_if_t<in_list_v<C, Cs...>, const C&>;

        template <typename C>
        auto has_component() const -> std::enable_if_t<in_list_v<C, Cs...>, bool>;

    private:
        World* world_;
        Entity entity_;
    };
}
