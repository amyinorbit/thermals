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
#include <tuple>

namespace amyinorbit::ecs {
    class World;

    template <typename... Cs>
    struct Proxy {
        using ComponentList = type_list<Cs...>;

        Proxy(World* world, Entity entity) : world_(world), entity_(entity) {}

        template <typename C>
        auto get_component() -> std::enable_if_t<in_list_v<C, Cs...>, C&>;

        template <typename C>
        auto get_component() const -> std::enable_if_t<in_list_v<C, Cs...>, const C&>;

        template <typename C>
        auto has_component() const -> std::enable_if_t<in_list_v<C, Cs...>, bool>;

        template <std::size_t N>
        nth_type_t<N, Cs...>& get() { return get_component<nth_type_t<N, Cs...>>(); }

        template <std::size_t N>
        const nth_type_t<N, Cs...>& get() const { return get_component<nth_type_t<N, Cs...>>(); }

    private:
        World* world_;
        Entity entity_;
    };
}

namespace std {
    template <typename... Cs>
    struct tuple_size<amyinorbit::ecs::Proxy<Cs...>>
        : std::integral_constant<std::size_t, sizeof...(Cs)> {};

    template <std::size_t N, typename... Cs>
    struct tuple_element<N, amyinorbit::ecs::Proxy<Cs...>> {
        using type = amyinorbit::ecs::nth_type_t<N, Cs...>;
    };
}
