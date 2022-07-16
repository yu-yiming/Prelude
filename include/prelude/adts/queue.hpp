#pragma once

#include "../defs.hpp"

namespace prelude {

template<typename T, typename Cont>
class queue {
public:
    using container_type = Cont;
    using iterator_type = Cont::iterator_type;
    using const_iterator_type = Cont::const_iterator_type;
    using pointer_type = T*;
    using size_type = prelude::size_t;
    using value_type = T;

    constexpr queue() noexcept(noexcept(container_type()))
        : container_type() {}

    constexpr queue(container_type const& cont)
        : m_cont(cont) {}

    constexpr queue(queue const& other) = default;

    constexpr queue(queue&& other) = default;

    static constexpr queue from_list(std::initializer_list<T> const& list) {
        return queue::from_range(list.begin(), list.end());
    }

    template<typename InIt>
    static constexpr queue from_range(InIt first, InIt last) {
        return { container_type::from_range(first, last) };
    }

    constexpr bool is_empty() const noexcept {
        return m_cont.is_empty();
    }

    constexpr T const& top() const {
        return m_cont.front();
    }

    constexpr T& top() {
        return m_cont.front();
    }

    constexpr void pop() {
        m_cont.pop_front();
    }

    constexpr void push(T const& val) {
        m_cont.push_back(val);
    }

protected:
    constexpr iterator_type begin() const {
        return m_cont.begin();
    }
    
    constexpr iterator_type end() const {
        return m_cont.end();
    }

private:
    container_type m_cont;
};


} // namespace prelude