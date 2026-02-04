//
// Created by Michael Hirsch on 04.02.26.
//

#pragma once
#include "functional"

template<typename Enum>
struct EnumHash {
    size_t operator()(Enum e) const noexcept {
        return static_cast<size_t>(e);
    };
};