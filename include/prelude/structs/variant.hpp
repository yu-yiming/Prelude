#pragma once

#include "../defs.hpp"
#include "../utils/type_list.hpp"

namespace prelude {

template<typename... Ts>
struct variant;

template<typename T, typename... Ts>
struct variant_element_wrapper;

template<typename... Ts>
struct variant_storage;

template<typename... Fs>
struct variant_visitor;


template<typename... Ts>
struct variant_storage {
    static constexpr prelude::size_t k_max_size = max_element<type_list<Ts...>>::value;

    // The index is 0 when the variant is empty.
    // So index is actually 1-indexed.
    unsigned char index = 0;

    constexpr void const* data() const noexcept {
        return m_data;
    }

    constexpr void* data() noexcept {
        return m_data;
    }

    template<typename T>
    constexpr T const* data_as() const noexcept {
        return std::launder(reinterpret_cast<T const*>(m_data));
    }

    template<typename T>
    constexpr T* data_as() noexcept {
        return std::launder(reinterpret_cast<T*>(m_data));
    }

private:
    // Make sure the size of the variant is at least the size of the largest variant element.
    alignas(Ts...) unsigned char m_data[k_max_size];
};

template<typename T, typename... Ts>
struct variant_element_wrapper {
    using value_type = T;
    using derived_type = variant<Ts...>;

    constexpr static prelude::size_t k_index = prelude::find<T, type_list<Ts...>>::value + 1;

    constexpr variant_element_wrapper() = default;

    constexpr variant_element_wrapper(T const& val) {
        auto& derived = this->as_derived();
        new (derived.data()) T(val);
        derived.index = k_index;
    }

    constexpr variant_element_wrapper(T&& val) {
        auto& derived = this->as_derived();
        new (derived.data()) T(prelude::move(val));
        derived.index = k_index;
    }

    constexpr derived_type const& as_derived() const noexcept {
        return static_cast<derived_type const&>(*this);
    }

    constexpr derived_type& as_derived() noexcept {
        return static_cast<derived_type&>(*this);
    }

    constexpr derived_type& operator =(T const& val) {
        auto& derived = this->as_derived();
        if (derived.index == k_index) {
            *derived.data_as<T>() = val;
        }
        else {
            derived.destroy();
            new (derived.data()) T(val);
            derived.index = k_index;
        }
        return derived;
    }

    constexpr derived_type& operator =(T&& val) {
        auto& derived = this->as_derived();
        if (derived.index == k_index) {
            *derived.data_as<T>() = val;
        }
        else {
            derived.destroy();
            new (derived.data()) T(prelude::move(val));
            derived.index = k_index;
        }
        return derived;       
    }

    constexpr bool try_destroy() {
        auto& derived = this->as_derived();
        if (derived.index == k_index) {
            derived.index = 0;
            derived.data_as<T>()->~T();
            return true;
        }
        return false;
    }

};

template<typename... Fs>
struct variant_visitor : private Fs... {
    using Fs::operator ()...;

    template<typeanme T>
    struct result_type_aux;

    template<typename... Ts, typename Test = void>
    using result_type = type_list<result_type_aux<Ts>::type...>;

    variant_visitor(Fs&&... fs)
        : Fs(prelude::move(fs))... {}

    template<typename... Ts>
    result_type<Ts...>::type operator ()(variant<Ts...>& var) const {

    }
};

template<>
template<typename T>
struct variant_visitor<>::result_type_aux<T> : type<null> {};

template<typename F, typename... Fs>
template<typename T>
struct variant_visitor<F, Fs...>::result_type_aux<T, prelude::void_t<decltype(prelude::declval<F>(std::declval<T>()))>> 
    : type<decltype(prelude::declval<F>(std::declval<T>()))> {};

template<typename F, typename... Fs>
template<typename T>
struct variant_visitor<F, Fs...>::result_type_aux<T> : variant_visitor<Fs...>::result_type_aux<T> {};

template<typename... Ts>
struct variant : private variant_storage<Ts...>, private variant_element_wrapper<Ts, Ts...>... {
    template<typename U, typename... Us>
    friend struct variant_wrapper;

    using storage_type = variant_storage<Ts...>;
    using element_type = variant_element_wrapper<Ts, Ts...>;

    using variant_element_wrapper<Ts, Ts...>::variant_element_wrapper...;
    using variant_element_wrapper<Ts, Ts...>::operator =...;

    constexpr variant();

    constexpr explicit variant(variant const& other);

    constexpr explicit variant(variant&& other);

    template<typename... Us>
    constexpr explicit variant(variant<Us...> const& other);

    template<typename... Us>
    constexpr explicit variant(variant<Us...>&& other);

    constexpr ~variant() {
        this->destroy();
    }

    void destroy() {
        (this->element_type<Ts, Ts...>::try_destroy(), ...);
    }

    template<typename T>
    constexpr T const& get() const& {
        if (this->is_empty()) {
            throw 42;
        }
        else if (!this->is<T>()) {
            throw 42;
        }
        return *this->template data_as<T>();
    }

    template<typename T>
    constexpr bool is() const {
        return static_cast<storage_type&>(*this).index == variant_element_wrapper<T, Ts...>::k_index;
    }

    constexpr bool is_empty() const noexcept;

    constexpr variant& operator =(variant const& other);

    constexpr variant& operator =(variant&& other);

    template<typename... Us>
    constexpr variant& operator =(variant<Us...> const& other);

    template<typename... Us>
    constexpr variant& operator =(variant<Us...>&& other);

    template<typename... Fs>
    auto visit(Fs&&... fs) & {
        return variant_visitor(prelude::forward<Fs>(fs)...);
    }
};



} // namespace prelude