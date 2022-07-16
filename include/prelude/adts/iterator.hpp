#pragma once

namespace prelude::iterator_adaptors {

template<typename It>
class reverse {
public:
    using value_type = It::value_type;
    using pointer_type = It::pointer_type;
    using const_pointer_type = It::const_pointer_type;
    using reference_type = It::reference_type;
    using const_reference_type = It::const_reference_type;
    using size_type = It::size_type;

    reverse(It it)
        : m_it(it) {}

    reverse(reverse const&) = default;

    reverse(reverse&&) = default;

    reverse& operator =(reverse const&) = default;

    reverse& operator =(reverse&&) = default;

    const_reference_type operator *() const {
        return *m_it;
    }

    reference_type operator *() {
        return *m_it;
    }

    const_pointer_type operator ->() const {
        return m_it.operator ->();
    }

    pointer_type operator ->() {
        return m_it.operator ->();
    }

    reverse& operator ++() {
        --m_it;
        return *this;
    }

    reverse operator ++(int) {
        auto ret = *this;
        --m_it;
        return ret;
    }

    reverse& operator --() {
        ++m_it;
        return *this;
    }

    reverse operator --(int) {
        auto ret = *this;
        ++m_it;
        return ret;
    }

    reverse& operator +=(size_type n) {
        m_it -= n;
        return *this;
    }

    reverse& operator -=(size_type n) {
        m_it += n;
        return *this;
    }

    reverse operator +(size_type n) const {
        return reverse(m_it - n);
    }

    reverse operator -(size_type n) const {
        return reverse(m_it + n);
    }

    reverse operator ==(reverse const& other) const {
        return m_it == other.m_it;
    }

private:
    It m_it;
};

template<typename It, typename Cont>
class push_back {

};



} // namespace prelude