//===--------------------------------------------------------------------------------------------===
// meta.hpp - Metaprogramming Utilities
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include "entity.hpp"
#include <type_traits>

namespace amyinorbit::ecs {

    template <typename... Ts> struct type_list {};

    inline Index &component_counter() {
      static Index counter = 0;
      return counter;
    }

    template<typename C>
    Index component_id() {
      static Index index = component_counter()++;
      return index;
    }

    template <typename C>
    TypeMask type_mask() {
        return 1 << component_id<C>();
    }

    template <typename C1, typename C2, typename... Cs>
    TypeMask type_mask() {
        return type_mask<C1>() | type_mask<C1, Cs...>();
    }

    // MARK: - Type list utilities
    // List inclusion check

    template <typename T, typename...>
    struct in_list;

    template <typename T>
    struct in_list<T, type_list<T>> : std::true_type {};

    template <typename T, typename... Us>
    struct in_list<T, type_list<T, Us...>> : std::true_type {};

    template <typename T, typename U>
    struct in_list<T, type_list<U>> : std::false_type {};

    template <typename T, typename U, typename... Vs>
    struct in_list<T, type_list<U, Vs...>> : in_list<T, type_list<Vs...>> {};

    template <typename T, typename... Us>
    constexpr bool in_list_v = in_list<T, type_list<Us...>>::value;

    // List element getting

    template <std::size_t N, typename... Ts>
    struct nth_type;

    template <typename T0, typename... Us>
    struct nth_type<0, type_list<T0, Us...>> { using type = T0; };

    template <std::size_t N, typename T0, typename... Us>
    struct nth_type<N, type_list<T0, Us...>> {
        using type = typename nth_type<N-1, type_list<Us...>>::type; };

    template <std::size_t N, typename... Ts>
    using nth_type_t = typename nth_type<N, type_list<Ts...>>::type;
}
