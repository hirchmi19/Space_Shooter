//
// Created by Michael Hirsch on 05.02.26.
//

#pragma once
#include <type_traits>

template<typename Enum>
constexpr size_t ToIndex(Enum e) {
    static_assert(std::is_enum_v<Enum>, "ToIndex requires an enum type");
    using UType = std::underlying_type_t<Enum>;
    return static_cast<size_t>(static_cast<UType>(e));
}