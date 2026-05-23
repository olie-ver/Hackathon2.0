#pragma once

#include <type_traits>
#include <utility>
#include <iterator>

namespace Concepts
{
    // =====================================================
    // has_begin_end
    // =====================================================

    template<typename, typename = void>
    struct has_begin_end : std::false_type {};

    template<typename T>
    struct has_begin_end<
        T,
        std::void_t<
            decltype(std::begin(std::declval<T>())),
            decltype(std::end(std::declval<T>()))
        >
    > : std::true_type {};

    // =====================================================
    // iterator value equality comparable
    // =====================================================

    template<typename, typename = void>
    struct iterator_value_equality : std::false_type {};

    template<typename T>
    struct iterator_value_equality<
        T,
        std::void_t<
            decltype(
                *std::begin(std::declval<T>())
                ==
                *std::begin(std::declval<T>())
            )
        >
    > : std::true_type {};

    // =====================================================
    // IterableComparable
    // =====================================================

    template<typename T>
    struct IterableComparable : std::integral_constant<
        bool,
        has_begin_end<T>::value &&
        iterator_value_equality<T>::value
    > {};

    template<typename T>
    inline constexpr bool IterableComparable_v =
        IterableComparable<T>::value;
}