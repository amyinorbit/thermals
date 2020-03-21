//===--------------------------------------------------------------------------------------------===
// proxy.inl - proxy implementation
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
// #include <ecs/world.hpp>

namespace amyinorbit::ecs {

    template <typename... Cs>
    template <typename C>
    inline auto Proxy<Cs...>::get_component() -> std::enable_if_t<in_list_v<C, Cs...>, C&> {
        return world_->get_component_unchk<C>(entity_);
    }

    template <typename... Cs>
    template <typename C>
    inline auto Proxy<Cs...>::get_component() const
    -> std::enable_if_t<in_list_v<C, Cs...>, const C&> {
        return world_->get_component_unchk<C>(entity_);
    }

    template <typename... Cs>
    template <typename C>
    inline auto Proxy<Cs...>::has_component() const
    -> std::enable_if_t<in_list_v<C, Cs...>, bool> {
        return world_->has_component<C>(entity_);
    }
}
