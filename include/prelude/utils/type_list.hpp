#pragma once

#include "general.hpp"

namespace prelude {

template<typename... Ts>
struct type_list;

template<template<typename, typename> class Op, typename List>
struct accumulate;

template<template<typename, typename> class Op, typename List, typename Acc>
struct accumulate_aux;

template<typename T, typename List>
struct contains;

template<typename T, typename List>
struct find;

template<typename T, typename List, prelude::size_t I>
struct find_aux;

template<typename List>
struct head;

template<typename List>
struct max_element;

template<typename List>
struct max_element_aux;

template<prelude::size_t I, typename List>
struct nth;

template<typename T, typename List>
struct prepend;

template<typename List>
struct size;

template<typename List>
struct tail;

template<template<typename> class F, typename List>
struct transform;

// Partial specializations for type_list<Ts...>

template<template<typename, typename> class Op, typename... Ts>
struct accumulate<Op, type_list<Ts...>> : accumulate_aux<Op, type_list<Ts...>, Acc> {};

template<template<typename, typename> class Op, typename Acc>
struct accumulate_aux<Op, type_list<>, Acc> : type<Acc> {};

template<template<typename, typename> class Op, typename Head, typename... Tail, typename Acc>
struct accumulate_aux<Op, type_list<Head, Tail...>, Acc> : accumulate<Op, type_list<Tail...>, typeof<Op<Acc, Head>>> {};

template<typename T>
struct contains<T, type_list<>> : constexpr_false {};

template<typename T, typename... Ts>
struct contains<T, type_list<T, Ts...>> : constexpr_true {};

template<typename T, typename Head, typename... Tail>
struct contains<T, type_list<Head, Tail...>> : contains<T, type_list<Tail...>> {};

template<typename T, typename... Ts>
struct find<T, type_list<Ts...>> : find_aux<T, type_list<Ts...>, 0> {};

template<typename T, prelude::size_t I>
struct find_aux<T, type_list<>, I> : constexpr_null {};

template<typename T, typename... Tail, prelude::size_t I>
struct find_aux<T, type_list<T, Tail...>, I> : constexpr_size<I> {};

template<typename T, typename Head, typename... Tail, prelude::size_t I>
struct find_aux<T, type_list<Head, Tail...>, I> : find_aux<T, type_list<Tail...>, I+1> {};

template<>
struct head<type_list<>> : null {};

template<typename Head, typename... Tail>
struct head<type_list<Head, Tail...>> : type<Head> {};

template<typename... Ts>
struct max_element<type_list<Ts...>> : max_element_aux<type_list<Ts...>, 0> {};

template<prelude::size_t N>
struct max_element_aux<type_list<>> : constexpr_size<N> {};

template<typename Head, typename... Tail, prelude::size_t N>
struct max_element_aux<type_list<Head, Tail...>, N> : max_element_aux<type_list<Tail...>, max(N, sizeof(Head))> {};

template<prelude::size_t I>
struct nth<I, type_list<>> : null {};

template<typename Head, typename... Tail>
struct nth<0, type_list<Head, Tail...>> : type<Head> {};

template<prelude::size_t I, typename Head, typename... Tail>
struct nth<I, type_list<Head, Tail...>> : nth<I-1, type_list<Tail...>> {};

template<typename... Ts>
struct size<type_list<Ts...>> : constexpr_size<sizeof...(Ts)> {};

template<typename Head, typename... Tail>
struct prepend<Head, type_list<Tail...>>, type<type_list<Head, Tail...>> {};

template<>
struct tail<type_list<>> : null {};

template<typename Head, typename... Tail>
struct tail<type_list<Head, Tail...>> : type<type_list<Tail...>> {};

template<template<typename> class F>
struct transform<F, type_list<>> : type<type_list<>> {};

template<template<typename> class F, typename Head, typename... Tail>
struct transform<F, type_list<Head, Tail...>> : prepend<typeof<F<Head>>, transform<F, type_list<Tail...>>> {};

} // namespace prelude