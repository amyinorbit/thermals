//===--------------------------------------------------------------------------------------------===
// World.hpp - Statically-sized Index-component manager
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
#include <cassert>
#include <cstdint>
#include <vector>
#include <memory>
#include <type_traits>
#include <ecs/entity.hpp>
#include <ecs/meta.hpp>
#include <ecs/proxy.hpp>
#include <ecs/view.hpp>

#define MAX_ENTITIES 64
#define MAX_COMPONENTS 16

namespace amyinorbit::ecs {
    using byte = std::uint8_t;
    struct Store {
        Store(std::size_t elt_size, std::size_t count);
        Store(Store&& other);
        Store& operator=(Store&& other);
        Store(const Store&) = delete;
        Store& operator=(const Store&) = delete;
        virtual ~Store();

        byte* ptr(Index index);
        const byte* ptr(Index index) const;
        virtual void destroy(Index index) = 0;
        virtual TypeMask mask() const = 0;
    private:
        void reset() {
            elt_size_ = 0;
            count_ = 0;
            data_ = nullptr;
        }
        std::size_t elt_size_ = 0;
        std::size_t count_ = 0;
        byte* data_ = nullptr;
    };

    template <typename T>
    struct TypedStore : Store {
        TypedStore() : Store(sizeof(T), MAX_ENTITIES) {}

        template <
            typename... Args,
            std::enable_if_t<!std::is_constructible_v<T, Args...>>* = nullptr
        >
        T& make(Index index, Args&&... args) {
            return *(new (ptr(index)) T{std::forward<Args>(args)...});
        }

        template <
            typename... Args,
            std::enable_if_t<std::is_constructible_v<T, Args...>>* = nullptr
        >
        T& make(Index index, Args&&... args) {
            return *(new (ptr(index)) T(std::forward<Args>(args)...));
        }

        virtual void destroy(Index index) { get(index).~T(); }
        T& get(Index index) { return *ptr(index); }
        const T& get(Index index) const { return *ptr(index); }

        T* ptr(Index index) { return reinterpret_cast<T*>(Store::ptr(index)); }
        const T* ptr(Index index) const { return reinterpret_cast<const T*>(Store::ptr(index)); }

        TypeMask mask() const { return type_mask<T>(); }
    private:
    };

    class World {
    public:

        World();

        Entity create();
        void destroy(Entity entity);

        Entity entity(Index idx) const { return Entity{idx, versions_[idx]}; }

        template <typename T, typename... Args>
        T& add_component(Entity entity, Args&&... args) {
            assert(is_valid(entity));
            masks_[entity.id] |= type_mask<T>();
            return store<T>().make(entity.id, std::forward<Args>(args)...);
        }

        template <typename T>
        void set_component(Entity entity, const T& component) {
            assert(is_valid(entity));
            assert(has_component<T>(entity));
            store<T>().get(entity.id) = component;
        }

        template <typename T>
        void set_component(Entity entity, T&& component) {
            assert(is_valid(entity));
            assert(has_component<T>(entity));
            store<T>().get(entity.id) = std::move(component);
        }

        template <typename T>
        T& get_component(Entity entity) {
            assert(is_valid(entity));
            assert(has_component<T>(entity));
            return store<T>().get(entity.id);
        }

        template <typename T>
        const T& get_component(Entity entity) const {
            assert(is_valid(entity));
            assert(has_component<T>(entity));
            return store<T>().get(entity.id);
        }

        template <typename T>
        T& get_component_unchk(Entity entity) { return store<T>().get(entity.id); }

        template <typename T>
        const T& get_component_unchk(Entity entity) const { return store<T>().get(entity.id); }

        template <typename T>
        bool has_component(Entity entity) const {
            return (masks_[entity.id] & type_mask<T>()) != 0;
        }

        template <typename... Ts>
        bool has_components(Entity entity) const {
            auto mask = type_mask<Ts...>();
            return (mask & masks_[entity.id]) == mask;
        }

        template <typename... Ts>
        bool has_components(Index id) const {
            auto mask = type_mask<Ts...>();
            return (mask & masks_[id]) == mask;
        }

        bool is_valid(Entity entity) const {
            return versions_[entity.id] == entity.version;
        }

        template <typename... Ts>
        View<EntityIterator<Ts...>> with() {
        using MyIterator = EntityIterator<Ts...>;
            using MyView = View<MyIterator>;
            return MyView{
                MyIterator(this, 0),
                MyIterator(this, MAX_COMPONENTS)
            };
        }

    private:

        template <typename T>
        TypedStore<T>& store() {
            auto idx = component_id<T>();
            return stores_[idx] ?
                *reinterpret_cast<TypedStore<T>*>(stores_[idx].get()) :
                create_store<T>();
        }

        template <typename T>
        TypedStore<T>& create_store() {
            auto id = component_id<T>();
            auto ptr = new TypedStore<T>();
            stores_[id].reset(ptr);
            return *ptr;
        }

        std::unique_ptr<Store> stores_[MAX_COMPONENTS];

        Index next_index_ = 0;

        TypeMask masks_[MAX_ENTITIES];
        Index versions_[MAX_ENTITIES];

        std::vector<Index> free_list_;
    };
}

#include "proxy.inl"
#include "view.inl"
