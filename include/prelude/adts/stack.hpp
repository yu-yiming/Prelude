#pragma once

#include "../defs.hpp"

namespace prelude {

template<typename T, typename Cont>
class stack {
public:
    using container_type = Cont;
    using iterator_type = Cont::iterator_type;
    using const_iterator_type = Cont::const_iterator_type;
    using pointer_type = T*;
    using size_type = prelude::size_t;
    using value_type = T;

    constexpr stack() noexcept(noexcept(container_type()))
        : container_type() {}

    constexpr stack(container_type const& cont)
        : m_cont(cont) {}

    constexpr stack(stack const& other) = default;

    constexpr stack(stack&& other) = default;

    static constexpr stack from_list(std::initializer_list<T> const& list) {
        return stack::from_range(list.begin(), list.end());
    }

    template<typename InIt>
    static constexpr stack from_range(InIt first, InIt last) {
        return { container_type::from_range(first, last) };
    }

    constexpr bool is_empty() const noexcept {
        return m_cont.is_empty();
    }

    constexpr T const& top() const {
        return m_cont.back();
    }

    constexpr T& top() {
        return m_cont.back();
    }

    constexpr void pop() {
        m_cont.pop_back();
    }

    constexpr void push(T const& val) {
        m_cont.push_back(val);
    }

    constexpr void push(T&& val) {
        m_cont.push_back(static_cast<T&&>(val));
    }

protected:
    constexpr iterator_type begin() const {
        return m_cont.begin();
    }

    constexpr const_iterator_type end() const {
        return m_cont.end();
    }

private:
    container_type m_cont;
};

} // namespace prelude