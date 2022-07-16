#pragma once

#include <type_traits>

#include "../defs.hpp"
#include "../algos/data.hpp"

namespace prelude {

// Constexpr value wrappers

template<typename T, T Val>
struct constexpr_value {
    static constexpr T value = Val;
};

template<bool Val>
using constexpr_bool = constexpr_value<bool, Val>;

template<int Val>
using constexpr_int = constexpr_value<int, Val>;

template<prelude::size_t Val>
using constexpr_size = constexpr_value<prelude::size_t, Val>;

using constexpr_null = constexpr_value<std::nullptr_t, nullptr>;

template<prelude::size_t N>
struct constexpr_string {
    char value[N];

    constexpr constexpr_string(char const (&arr)[N]) {
        prelude::copy(arr, arr + N, value);
    }
};

using constexpr_true = constexpr_bool<true>;

using constexpr_false = constexpr_bool<false>;

template<typename ExprA, typename ExprB>
struct both : constexpr_bool<ExprA::value && ExprB::value> {};

template<typename ExprA, typename ExprB>
struct either : constexpr_bool<ExprA::value || ExprB::value> {};

template<typename ExprA, typename ExprB>
struct neither : constexpr_bool<!ExprA::value && !ExprB::value> {};

template<typename Expr>
constexpr auto valueof = Expr::value;

// Type wrapper

template<typename... Ts>
struct void_t {};

template<typename T, typename Test = void>
struct type {
    using type = T;
};

template<typename T>
struct type<T, prelude::void_t<typename T::type>> {
    using type = T::type;
};

template<typename T>
using typeof = type<T>::type;

using null = type<std::nullptr_t>;

// Type operations

template<typename Cond, typename Then, typename Else>
struct branch;

template<typename Then, typename Else>
struct branch<constexpr_bool<true>, Then, Else> : type<Then> {};

template<typename Then, typename Else>
struct branch<constexpr_bool<false>, Then, Else> : type<Else> {};

template<typename T, prelude::size_t N>
consteval T const_value(char const (&arr)[N]) {
    auto const digit = [](char c) {
        if (c >= 'A' && c <= 'F') {
            return T(c) - T('A') + 10;
        }
        else if (c >= 'a' && c <= 'f') {
            return T(c) - T('a') + 10;
        }
        return T(c) - T('0');
    };

    auto base = 10;
    auto offset = 0;

    if (N > 2 && arr[0] == '0') {
        switch (arr[1]) {
        case 'B':
        case 'b':
            base = 2;
            offset = 2;
            break;
        case 'X':
        case 'x':
            base = 16;
            offset = 2;
            break;
        default:
            base = 8;
            offset = 1;
            break;
        }
    }

    T result = 0;
    T power = 1;
    for (auto i = 0uz; i < N - offset; ++i) {
        if (arr[N-i-1] != '\'') {
            result = digit(arr[N-i-1]) * power;
            power *= base;
        }
    }
    return result;
}

template<typename T, char... Cs>
consteval T const_value() {
    return const_value<T>({ Cs... });
}

template<typename T>
constexpr T&& declval();

template<typename T>
constexpr T&& forward(std::remove_reference_t<T>& t) noexcept {
    return static_cast<T&&>(t);
}

template<typename T>
constexpr T&& forward(std::remove_reference_t<T>&& t) noexcept {
    return static_cast<T&&>(t);
}

template<typename T>
constexpr T&& move(T& t) noexcept {
    return static_cast<T&&>(t);
}


} // namespace prelude