//===--------------------------------------------------------------------------------------------===
// view.hpp - Implementation of views/iterators
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===

namespace amyinorbit::ecs {

    template <typename... Cs>
    auto EntityIterator<Cs...>::operator++() -> EntityIterator& {
        index_++;
        next();
        return *this;
    }

    template <typename... Cs>
    auto EntityIterator<Cs...>::operator*() const -> reference {
        return reference(world_, world_->entity(index_));
    }

    template <typename... Cs>
    void EntityIterator<Cs...>::next() {
        while(!world_->has_components<Cs...>(index_) && index_ != MAX_COMPONENTS) {
            index_ += 1;
        }
    }
}
