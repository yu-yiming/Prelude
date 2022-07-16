#pragma once

#include "../defs.hpp"
#include "../utils/meta.hpp"

namespace prelude {

template<typename R, typename List>
struct function_signature;

template<prelude::size_t I, typename FSig>
struct nth_argument;

template<prelude::size_t I, typename R, typename List>
struct nth_argument<I, function_signature<R, List>> : nth<I, List> {};

template<typename FSig>
struct return_value;

template<typename R, typename List>
struct return_value<function_signature<R, List>> : type<R> {};



template<typename Sig>
class function;

template<typename R, typename... Args>
struct function_base {
    virtual ~function_base() = default;

    virtual function_base* clone() const = 0;

    virtual R operator ()(Args&&... args) const = 0;

    virtual bool operator ==(function_base<R, Args...> const& other) const = 0;
};

template<typename F, typename R, typename... Args>
struct function_wrapper : function_base<R, Args...> {
    F func;

    template<typename G>
    function_wrapper(G&& g)
        : func(prelude::move(g)) {}

    function_wrapper* clone() const override {
        return new function_wrapper(func);
    }

    R operator ()(Args... args) const override {
        return func(prelude::forward<Args>(args)...);
    }

    bool operator ==(function_base<R, Args...> const& other) const override {
        if (auto const* other_ptr = dynamic_cast<function_wrapper const*>(&other)) {
            return func == other_ptr->func;
        }
        return false;
    }
};

template<typename R, typename... Args>
class function<R (Args...)> {
public:
    using return_type = R;
    using parameter_types = type_list<Args...>;

    template<typename F>
    function(F&& f)
        : m_fptr(new function_wrapper<F, R, Args...>(prelude::forward<typeof<prelude::decay<F>>>(f))) {}

    function(function const& other) {
        if (other.m_fptr != nullptr) {
            m_fptr = other.m_fptr->clone();
        }
    }

    function(function&& other)
        : m_fptr(other.m_fptr) {

        other.m_fptr = nullptr;
    }

    ~function() {
        delete m_fptr;
    }

    function& operator =(function const& other) {
        auto tmp = other;
        swap(*this, tmp);
        return *this;
    }

    function& operator =(function&& other) {
        delete m_fptr;
        m_fptr = other.m_fptr;
        other.m_fptr = nullptr;
        return *this;
    }

    R operator ()(Args... args) {
        if (m_fptr != nullptr) {
            return m_fptr->operator ()(prelude::forward<Args>(args)...);
        }
        return R();
    }

    friend bool operator ==(function const& lhs, function const& rhs) {
        return lhs.m_fptr == rhs.m_fptr || *lhs.m_fptr == *rhs.m_fptr;
    }

    explicit operator bool() const noexcept {
        return m_fptr != nullptr;
    }

    friend void swap(function& lhs, function& rhs) {
        std::swap(lhs.m_fptr, rhs.m_fptr);
    }

private:
    function_base<R, Args...>* m_fptr;
};

} // namespace prelude