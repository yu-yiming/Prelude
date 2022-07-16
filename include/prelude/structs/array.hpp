#pragma once

#include <initializer_list>
#include <utility>

#include "../defs.hpp"
#include "../algos/data.hpp"

namespace prelude {

template<typename T, prelude::size_t N>
struct array {
    static_assert(N > 0, "Cannot create array of size 0");

    using value_type = T;
    using size_type = prelude::size_t;

    T data[N];

    constexpr array() noexcept {
        prelude::fill(data, data + N, T{});
    }

    template<typename... Args>
    constexpr array(Args&&... args) noexcept {
        static_assert(sizeof...(Args) <= N, "Too many arguments for the array");
        prelude::fill_args(data, static_cast<Args&&>(args)...);
    }

    constexpr T const& back() const {
        return data[N - 1];
    }

    constexpr T& back() {
        return data[N - 1];
    }

    constexpr T const* begin() const noexcept {
        return data;
    }

    constexpr T* begin() noexcept {
        return data;
    }

    constexpr T const* cbegin() const noexcept {
        return data;
    }

    constexpr T const* cend() const noexcept {
        return data + N;
    }

    constexpr T const* end() const noexcept {
        return data + N;
    }

    constexpr T* end() noexcept {
        return data + N;
    }

    static constexpr array from_list(std::initializer_list<T> const& list) {
        return array::from_range(list.begin(), list.end());
    }

    template<typename InIt>
    static constexpr array from_range(InIt first, InIt last) {
        auto result = array();
        prelude::copy(first, last, result.data);
        return result;
    }

    constexpr T const& front() const {
        return data[0];
    }

    constexpr T& front() {
        return data[0];
    }

    constexpr bool is_empty() const noexcept {
        return false;
    }

    constexpr T const& operator [](size_type i) const {
        return data[i];
    }

    constexpr T& operator [](size_type i) {
        return data[i];
    }
};

template<typename T, prelude::size_t N, prelude::size_t M>
constexpr array<T, N + M> concat(array<T, N> const& arr, array<T, M> const& brr) {
    auto result = array<T, N + M>();
    prelude::copy(arr.begin(), arr.end(), result.data);
    prelude::copy(brr.begin(), brr.end(), result.data + N);
    return result;
}

// Support for structured bindings
template<typename T, prelude::size_t N>
struct std::tuple_size<array<T, N>> : std::integral_constant<prelude::size_t, N> {};

template<prelude::size_t I, typename T, prelude::size_t N>
struct std::tuple_element<I, array<T, N>> {
    static_assert(I < N, "Index out of bound");
    using type = T;
};

template<prelude::size_t I, typename T, prelude::size_t N>
constexpr std::tuple_element_t<I, array<T, N>>& get(array<T, N>& arr) {
    return arr[I];
}


} // namespace prelude