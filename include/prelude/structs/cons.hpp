#pragma once

#include "../defs.hpp"
#include "../algos/data.hpp"
#include "../utils/allocator.hpp"

namespace prelude {

template<typename T>
struct cons_iterator;

template<typename T>
using cons_const_iterator = cons_iterator<T>;

template<typename T, typename Alloc = prelude::node_allocator<cons<T>>>
struct cons {
    using value_type = T;
    using size_type = prelude::size_t;
    using iterator_type = cons_iterator<T>;
    using const_iterator_type = cons_const_iterator<T>;

    T head;
    void* tail;
    [[no_unique_address]] Alloc alloc = {};

    constexpr cons(T const& head, cons<T>* tail = nullptr) noexcept
        : head(head), tail(tail) {}

    constexpr cons(T&& head, cons<T>* tail = nullptr) noexcept
        : head(static_cast<T&&>(head)), tail(tail) {}

    constexpr cons(T const& head, cons<void>* tail) noexcept
        : head(head), tail(tail) {}

    constexpr cons(T&& head, cons<void>* tail) noexcept
        : head(static_cast<T&&>(head)), tail(tail) {}

    constexpr T const& back() const {
        auto* it = this;
        while (it->tail != nullptr) {
            it = static_cast<cons<T>*>(it->tail);
        }
        return it->head;
    }

    const_iterator_type begin() const noexcept;

    iterator_type begin() noexcept {
        return { static_cast<cons<T> const&>(*this).begin() };
    }

    const_iterator_type cbegin() const noexcept {
        return this->begin();
    }

    constexpr const_iterator_type cend() const noexcept {
        return this->end();
    }

    constexpr iterator_type end() const noexcept;

    constexpr const_iterator_type end() noexcept {
        return { static_cast<cons<T> const&>(*this).end() };
    }

    // TODO: write a push_node iterator adaptor for this.
    static constexpr cons<T> from_list(std::initializer_list<T> list) {
        return cons<T>::from_range(list.begin(), list.end());
    }

    template<typename InIt, typename InIt>
    static constexpr cons<T> from_range(InIt first, InIt last) {
        auto* sentinel = alloc.allocate(1);
        auto* it = sentinel;
        while (first != last) {
            it->tail = alloc.new_object(*first++);
            it = static_cast<cons<T>*>(it->tail);
        }
        auto result = sentinel->tail;
        alloc.deallocate(sentinel, 1);
        return result;
    }

    constexpr T const& front() const noexcept {
        return head;
    }

    constexpr T const& operator [](size_type i) const {
        if (i == 0) {
            return head;
        }
        return static_cast<cons<T>*>(tail)[i - 1];
    }

    constexpr cons<T> operator +(cons<T> const& other) const {
        
    }

};

template<>
struct cons<void> {
    using value_type = void;
    using size_type = prelude::size_t;

    constexpr cons() noexcept {}
};

using nil_t = cons<void>;

} // namespace prelude