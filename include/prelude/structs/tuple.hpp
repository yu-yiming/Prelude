#pragma once

#include <utility>

#include "../defs.hpp"
#include "../utils/general.hpp"
#include "../utils/meta.hpp"

namespace prelude {

template<typename... Ts>
struct tuple;

/**
 * @brief The tuple element wrapper. This is just a wrapper around any type, and we can access
 * its data through the `get()` member function. We need this wrapper to ensure the member layout
 * in struct tuple (so that the tuple elements will be initialized in order).
 * 
 * @tparam T The underlying type.
 * @tparam I The position of the element. This prevents identical tuple element types, in which 
 * case it will be difficult to access certain tuple elements (since our tuple is implemented
 * with chaining inheritance).
 */
template<typename T, prelude::size_t I>
struct tuple_element_wrapper;

template<char... Cs>
consteval auto operator ""_idx();

template<typename T, prelude::size_t I>
struct tuple_element_wrapper {
    template<typename U>
    tuple_element_wrapper(U&& val)
        : m_value(prelude::forward<U>(val)) {}

    T const& get() const noexcept {
        return m_value;
    }

    T& get() noexcept {
        return m_value;
    }

private:
    [[no_unique_address]]
    T m_value;
};

template<>
struct tuple<> {
    // Empty
};

/**
 * @brief The tuple class. Implemented with chaining inheritance, i.e. tuple<Head, Tail...> inherits
 * from tuple<Tail...>. To ensure the order of initialization matches the order of the elements, we 
 * also let it inherit from the element wrapper of Head.
 * 
 * @tparam Head 
 * @tparam Tail 
 */
template<typename Head, typename... Tail>
struct tuple<Head, Tail...> : tuple_element_wrapper<Head, sizeof...(Tail)>, tuple<Tail...> {
    using head_type = Head;
    using head_element_type = tuple_element_wrapper<head_type, sizeof...(Tail)>;
    using tail_type = tuple<Tail...>;

    constexpr tuple() = default;

    template<typename Head_, typename... Tail_>
    constexpr tuple(Head_&& head, Tail_&&... tail)
        : head_element_type(prelude::forward<Head_>(head)),
          tail_type(prelude::forward<Tail_>(tail)...) {}

    constexpr head_type const& head() const noexcept {
        return static_cast<head_element_type const&>(*this).get();
    }

    constexpr head_type& head() noexcept {
        return static_cast<head_element_type&>(*this).get();
    }

    /**
     * @brief Access a tuple element using the subscript operator. The parameter type is a compile-time
     * index wrapper class. See the following examples for more details:
     * 
     * @example Use constexpr_size<> to indicate the index.
     * @code
     *      auto tup = tuple(1, "abc", 2.0);
     *      std::cout << tup[constexpr_size<1>()] << std::endl; // prints "abc"
     * @endcode
     * @example Use ""_idx to indicate the index.
     * @code
     *      auto tup = tuple(1, "abc", 2.0);
     *      std::cout << tup[1_idx] << std::endl; // prints "abc"
     * @endcode
     */
    template<prelude::size_t I>
    constexpr auto operator [](constexpr_size<I>);

    constexpr tail_type const& tail() const noexcept {
        return *this;
    }

    constexpr tail_type& tail() noexcept {
        return *this;
    }
};

// Support for structured bindings
template<typename... Ts>
struct std::tuple_size<tuple<Ts...>> : std::integeral_constant<prelude::size_t, sizeof...(Ts)> {};

template<typename... Ts, prelude::size_t I>
struct std::tuple_element<I, tuple<Ts...>> {
    static_assert(I < sizeof...(Ts), "Index out of bound");
    using type = std::tuple_element_t<I, tuple<Ts...>>;
};

template<prelude::size_t I, typename T>
constexpr T& get_element(tuple_element_wrapper<T, I>& te) {
    return te.get();
}

template<prelude::size_t I, typename... Ts>
constexpr auto get(tuple<Ts...>& tup) {
    return get_element<sizeof...(Ts) - I - 1>(tup);
}

template<typename Head, typename... Tail>
template<prelude::size_t I>
constexpr auto tuple<Head, Tail...>::operator [](constexpr_size<I>) {
    return get<I>(*this);
}

template<char... Cs>
consteval auto operator ""_idx() {
    constexpr char arr[] = { Cs... };
    return constexpr_size<const_value<prelude::size_t>(arr)>();
}



} // namespace prelude