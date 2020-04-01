//===--------------------------------------------------------------------------------------------===
// World.cpp - entity-component manager
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <ecs/world.hpp>
#ifndef NDEBUG
#include <iostream>
#endif

namespace amyinorbit::ecs {

    Store::Store(std::size_t elt_size, std::size_t count) : elt_size_(elt_size), count_(count) {
        data_ = new byte[elt_size * count];
    }

    Store::Store(Store&& other) : elt_size_(other.elt_size_), count_(other.count_) {
        data_ = other.data_;
        other.reset();
    }

    Store& Store::operator=(Store&& other) {
        if(&other != this) {
            delete [] data_;
            elt_size_ = other.elt_size_;
            count_ = other.count_;
            data_ = other.data_;
            other.reset();
        }
        return *this;
    }

    Store::~Store() {
        delete [] data_;
        reset();
    }

    byte* Store::ptr(Index index) {
        return data_ + index * count_;
    }

    const byte* Store::ptr(Index index) const {
        return data_ + index * count_;
    }

    World::World() {
        for(Index id = 0; id < MAX_ENTITIES; ++id) {
            masks_[id] = 0;
            versions_[id] = 0;
            stores_[0] = nullptr;
        }
    }

    Entity World::create() {
        if(free_list_.size()) {
            auto idx = free_list_.back();
            free_list_.pop_back();
            std::cout << "Creating from free list: " << idx << "\n";
            return Entity{idx, versions_[idx]};
        }
        assert(next_index_ < MAX_ENTITIES);

        auto id = next_index_++;
        versions_[id] = 1;
        std::cout << "Creating new entity: " << id << "\n";
        return Entity{id, versions_[id]};
    }

    void World::destroy(Entity entity) {
        assert(is_valid(entity));
        auto cmask = masks_[entity.id];
        for(auto& store: stores_) {
            if(!store) continue;
            if(!(store->mask() & cmask)) continue;
            store->destroy(entity.id);
        }

        masks_[entity.id] = 0;
        versions_[entity.id] += 1;
        free_list_.push_back(entity.id);
    }
}
