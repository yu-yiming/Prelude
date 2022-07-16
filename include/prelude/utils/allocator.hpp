#pragma once

#include "../defs.hpp"

namespace prelude {

template<typename T>
class basic_allocator {
public:
    using value_type = T;
    using pointer_type = Ptr;
    using size_type = prelude::size_t;

    basic_allocator() noexcept = default;

    [[nodiscard]]
    T* allocate(size_type n) {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    template<typename... Args>
    T* construct(T* ptr, Args&&... args) {
        return ::new(ptr) T(static_cast<Args&&>(args)...);
    }

    void deallocate(T* p, size_type n) {
        ::operator delete(p);
    }

    template<typename... Args>
    T* new_object(Args&&... args) {
        return construct(allocate(1), static_cast<Args&&>(args)...);
    }
};

template<typename Node>
class node_allocator {
public:
    using value_type = Node::value_type;
    using pointer_type = Node*;
    using size_type = prelude::size_t;

    node_allocator() noexcept = default;

    [[nodiscard]]
    Node* allocate(size_type n) {
        return static_cast<Node*>(::operator new(n * sizeof(Node)));
    }

    template<typename... Args>
    Node* construct(Node* ptr, Args&&... args) {
        return ::new(ptr) Node(static_cast<Args&&>(args)...);
    }

    void deallocate(Node* p, size_type n) {
        ::operator delete(p);
    }

    template<typename... Args>
    Node* new_object(Args&&... args) {
        return construct(allocate(1), static_cast<Args&&>(args)...);
    }
};


} // namespace prelude