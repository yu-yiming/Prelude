#pragma once

#include <type_traits>

#include "general.hpp"
#include "type_list.hpp"

namespace prelude {

namespace trait_tags {

template<typename R, typename... Args>
struct function {
    using category = function;
    using return_type = R;
    using parameter_types = type_list<Args...>;
    static constexpr bool is_member_function = false;
};

template<typename C, typename R, typename... Args>
struct member_function {
    using category = member_function<C>;
    using class_type = C;
    using return_type = R;
    using parameter_types = type_list<Args...>;
    static constexpr bool is_member_function = true;
};

struct variadic {
    static constexpr bool is_variadic = true;
};

} // namespace trait_tags

// Forward declarations

template<typename FSig>
struct function_traits;

template<typename FSig, bool Test = function_traits<FSig>::is_member_function>
struct universal_function;

template<typename T>
struct remove_volatile;

template<typename T>
struct is_rvalue_reference;

template<prelude::size_t I, typename FSig>
struct nth_argument;

template<typename FSig>
struct return_value;

// Type functions

template<typename T>
struct add_lvalue_reference : type<T&> {};

template<typename T>
struct add_rvalue_reference : type<T&&> {};

template<typename T, typename U>
struct common_op : type<std::declval<T>() ? std::declval<U>() : std::declval<T>()> {};

template<typename... Ts>
struct common_type : accumulate<common_op, type_list<Ts...>> {};

template<typename T>
struct decay : remove_volatile<T> {};

template<typename T>
struct decay<T []> : type<T*> {};

template<typename T, prelude::size_t>
struct decay<T [N]> : type<T*> {};

template<typename R, typename... Args>
struct decay<R (Args...)> : type<R (*)(Args...)> {};

template<typename R, typename... Args>
struct decay<R (Args..., ...)> : type<R (*)(Args..., ...)> {};

template<typename T>
struct remove_const : type<T> {};

template<typename T>
struct remove_const<T const> : type<T> {};

template<typename T>
struct remove_const_volatile : remove_volatile<remove_const<T>::type> {};

template<typename T>
struct remove_volatile : type<T> {};

template<typename T>
struct remove_volatile<T volatile> : type<T> {};

template<typename T>
struct remove_reference : type<T> {};

template<typename T>
struct remove_reference<T&> : type<T> {};

template<typename T>
struct remove_reference<T&&> : type<T> {};

template<typename FSig>
struct universal_function<FSig, false> {
    using trait_type = function_traits<FSig>;
    using return_type = trait_type::return_type;
    using parameter_types = trait_type::parameter_types;
};

template<typename FSig>
struct universal_function<FSig, true> {
    using trait_type = function_traits<FSig>;
    using return_type = trait_type::return_type;
    using parameter_types = typeof<prepend<trait_type::class_type, trait_type::parameter_types>>;
};

// Type traits

template<typename T>
concept class_type = std::is_class_v<T>;

template<typename T>
concept final_type = std::is_final_v<T>;

#pragma region Overloading Disaster

template<typename R, typename... Args>
struct function_traits<R (Args...)> : trait_tags::function<R, Args...> {};

template<typename R, typename... Args>
struct function_traits<R (Args..., ...)> : traits_tag::function<R, Args...>, trait_tags::variadic {};

template<typename R, typename... Args>
struct function_traits<R (*)(Args...)> : trait_tags::function<R, Args...> {};

template<typename R, typename... Args>
struct function_traits<R (*)(Args..., ...)> : traits_tag::function<R, Args...>, trait_tags::variadic {};

template<typename R, typename... Args>
struct function_traits<R (* const)(Args...)> : trait_tags::function<R, Args...> {};

template<typename R, typename... Args>
struct function_traits<R (* const)(Args..., ...)> : traits_tag::function<R, Args...>, trait_tags::variadic {};

template<typename R, typename... Args>
struct function_traits<R (* volatile)(Args...)> : trait_tags::function<R, Args...> {};

template<typename R, typename... Args>
struct function_traits<R (* volatile)(Args..., ...)> : traits_tag::function<R, Args...>, trait_tags::variadic {};

template<typename R, typename... Args>
struct function_traits<R (* const volatile)(Args...)> : trait_tags::function<R, Args...> {};

template<typename R, typename... Args>
struct function_traits<R (* const volatile)(Args..., ...)> : traits_tag::function<R, Args...>, trait_tags::variadic {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args...)> : trait_tags::member_function<C, R, Args...> {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args..., ...)> : traits_tag::member_function<C, R, Args...>, trait_tags::variadic {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args...) const> : trait_tags::member_function<C const, R, Args...> {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args..., ...) const> : traits_tag::member_function<C const, R, Args...>, trait_tags::variadic {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args...) volatile> : trait_tags::member_function<C volatile, R, Args...> {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args..., ...) volatile> : traits_tag::member_function<C volatile, R, Args...>, trait_tags::variadic {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args...) const volatile> : trait_tags::member_function<C const volatile, R, Args...> {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args..., ...) const volatile> : traits_tag::member_function<C const volatile, R, Args...>, trait_tags::variadic {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args...) &> : trait_tags::member_function<C&, R, Args...> {};

template<typename R, typename C, typename... Arg>
struct function_traits<R (C::*)(Args..., ...) &> : traits_tag::member_function<C&, R, Args...>, trait_tags::variadic {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args...) const&> : trait_tags::member_function<C const&, R, Args...> {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args..., ...) const&> : traits_tag::member_function<C const&, R, Args...>, trait_tags::variadic {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args...) volatile&> : trait_tags::member_function<C volatile&, R, Args...> {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args..., ...) volatile&> : traits_tag::member_function<C volatile&, R, Args...>, trait_tags::variadic {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args...) const volatile&> : trait_tags::member_function<C const volatile&, R, Args...> {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args..., ...) const volatile&> : traits_tag::member_function<C const volatile&, R, Args...>, trait_tags::variadic {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args..) &&> : trait_tags::member_function<C&&, R, Args...> {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args..., ...) &&> : traits_tag::member_function<C&&, R, Args...>, trait_tags::variadic {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args...) const&&> : trait_tags::member_function<C const&&, R, Args...> {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args..., ...) const&&> : traits_tag::member_function<C const&&, R, Args...>, trait_tags::variadic {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args...) volatile&&> : trait_tags::member_function<C volatile&&, R, Args...> {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args..., ...) volatile&&> : traits_tag::member_function<C volatile&&, R, Args...>, trait_tags::variadic {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args...) const volatile&&> : trait_tags::member_function<C const volatile&&, R, Args...> {};

template<typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args..., ...) const volatile&&> : traits_tag::member_function<C const volatile&&, R, Args...>, trait_tags::variadic {};

#pragma endregion // Overloading Disaster

template<typename T>
struct is_array : constexpr_false {};

template<typename T>
struct is_array<T []> : constexpr_true {};

template<typename T, prelude::size_t N>
struct is_array<T [N]> : constexpr_true {};

template<typename T, typename Test = void>
struct is_class : constexpr_false {};

template<typename T>
struct is_class<T, void_t<int T::*>> : constexpr_true {};

template<typename T, typename Test = void>
struct is_default_constructible : constexpr_false {};

template<typename T>
struct is_default_constructible<T, prelude::void_t<decltype(T())>> {};

template<typename T>
struct is_function : constexpr_false {};

template<typename R, typename... Args>
struct is_function<R (Args...)> : constexpr_true {};

template<typename R, typename... Args>
struct is_function<R (Args..., ...)> : constexpr_true {};

template<typename T>
struct is_lvalue_reference : constexpr_false {};

template<typename T>
struct is_lvalue_reference<T&> : constexpr_true {};

template<typename T>
struct is_member_pointer : constexpr_false {};

template<typename R, typename C>
struct is_member_pointer<R C::*> : constexpr_true {};

template<typename T, typename Test = void>
struct is_memeber_function_pointer : constexpr_false {};

template<typename T>
struct is_member_function_pointer<T, prelude::void_t<function_traits<T>>> : constexpr_bool<function_traits<T>::is_member_function> {};

template<typename T>
struct is_pointer : constexpr_false {};

template<typename T>
struct is_pointer<T*> : constexpr_true {};

template<typename T>
struct is_reference : either<is_lvalue_reference<T>, is_rvalue_reference<T>> {};

template<typename T>
struct is_rvalue_reference : constexpr_false {};

template<typename T>
struct is_rvalue_reference<T&&> : constexpr_true {};

template<typename T, typename U>
struct is_same : constexpr_false {};

template<typename T>
struct is_same<T, T> : constexpr_true {};

template<prelude::size_t I>
struct nth_argument<I, R ()> : null {};

template<typename R, typename Head, typename... Tail>
struct nth_argument<0, R (Head, Tail...)> : type<Head> {};

template<prelude::size_t I, typename R, typename Head, typename... Tail>
struct nth_argument<I, R (Head, Tail...)> : nth_argument<I-1, R (Tail...)> {};

template<typename R, typename... Args>
struct return_value<R (Args...)> : type<R> {};

template<typename T, typename U>
concept same_type<T, U> = is_same<T, U>::value;

} // namespace prelude