//
// Created by Michael Hirsch on 05.02.26.
//

#pragma once
#include <type_traits>


/**
 * Casts an enumeration type into a size_t integer
 * \tparam Enum
 * \param e
 * \return
 */
template<typename Enum>
constexpr size_t ToIndex(Enum e) {
    static_assert(std::is_enum_v<Enum>, "ToIndex requires an enum type");
    using UType = std::underlying_type_t<Enum>;
    return static_cast<size_t>(static_cast<UType>(e));
}

template<typename Enum>
constexpr Enum ToEnum(size_t value) {

    static_assert(std::is_enum_v<Enum>, "ToEnumChecked requires an enum type");

    if (value >= static_cast<size_t>(Enum::COUNT)) {
        throw std::out_of_range("Invalid enum index");
    }

    return static_cast<Enum>(value);
}