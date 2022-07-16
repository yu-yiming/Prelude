#pragma once

#include "../defs.hpp"

namespace prelude {

template<typename T>
struct singly_linked_node {
    T data;
    singly_linked_node* next;
};

template<typename T>
struct doubly_linked_node {
    T data;
    doubly_linked_node* next;
    doubly_linked_node* prev;
};

template<typename T, prelude::size_t N>
struct tree_node {
    T data;
    tree_node<T, N>* children[N];
};

template<typename T>
struct binary_tree_node {
    T data;
    binary_tree_node<T>* left;
    binary_tree_node<T>* right;
};



template<typename T>
constexpr void insert_after(singly_linked_node<T>* pos, singly_linked_node<T>* node) {
    if (pos == nullptr) {
        return;
    }
    node->next = pos->next;
    pos->next = node;
}

template<typename T>
constexpr void insert_after(doubly_linked_node<T>* pos, doubly_linked_node<T>* node) {
    if (pos == nullptr) {
        return;
    }
    node->next = pos->next;
    node->next->prev = node;
    pos->next = node;
    node->prev = pos;
}

template<typename T>
constexpr singly_linked_node<T>* last(singly_linked_node<T>* head) {
    if (head == nullptr) {
        return nullptr;
    }
    while (head->next != nullptr) {
        head = head->next;
    }
    return head;
}

template<typename T>
constexpr doubly_linked_node<T>* last(doubly_linked_node<T>* head) {
    return reinterpret_cast<doubly_linked_node<T>*>(last(reinterpret_cast<singly_linked_node<T>*>(head)));
}

template<typename T, prelude::size_t N>
constexpr prelude::size_t height(tree_node<T, N>* root) {
    if (root == nullptr) {
        return 0;
    }
    prelude::size_t max_height = 0;
    for (prelude::size_t i = 0; i < N; ++i) {
        max_height = std::max(max_height, height(root->children[i]));
    }
    return max_height + 1;
}

template<typename T>
constexpr singly_linked_node<T>* middle(singly_linked_node<T>* head) {
    if (head == nullptr) {
        return nullptr;
    }
    auto* slow = head;
    auto* fast = head;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

template<typename T>
constexpr doubly_linked_node<T>* middle(doubly_linked_node<T>* head) {
    return reinterpret_cast<doubly_linked_node<T>*>(reinterpret_cast<singly_linked_node<T>*>(head));
}

template<typename T>
constexpr singly_linked_node<T>* merge(singly_linked_node<T>* head_1, singly_linked_node<T>* head_2, auto&& pred) {
    if (head_1 == nullptr) {
        return head_2;
    }
    else if (head_2 == nullptr) {
        return head_1;
    }

    auto dumb = singly_linked_node<T> { T(), nullptr };
    auto* it = &dumb;

    while (head_1 != nullptr && head_2 != nullptr) {
        if (pred(head_1->data, head_2->data)) {
            it->next = head_1;
            head_1 = head_1->next;
            it = head_1;
        }
        else {
            it->next = head_2;
            head_2 = head_2->next;
            it = head_2;
        }
    }

    if (head_1 != nullptr) {
        it->next = head_1;
        head_1->prev = it;
    }
    else if (head_2 != nullptr) {
        it->next = head_2;
        head_2->prev = it;
    }
    return dumb.next;
}

template<typename T>
constexpr doubly_linked_node<T>* merge(doubly_linked_node<T>* head_1, doubly_linked_node<T>* head_2, auto&& pred) {
    if (head_1 == nullptr) {
        return head_2;
    }
    else if (head_2 == nullptr) {
        return head_1;
    }

    auto dumb = doubly_linked_node<T> { T(), nullptr, nullptr };
    auto* it = &dumb;

    while (head_1 != nullptr && head_2 != nullptr) {
        if (pred(head_1->data, head_2->data)) {
            it->next = head_1;
            if (head_1 != nullptr) {
                head_1->prev = it;
            }
            head_1 = head_1->next;
            it = head_1;
        }
        else {
            it->next = head_2;
            if (head_2 != nullptr) {
                head_2->prev = it;
            }
            head_2 = head_2->next;
            it = head_2;
        }
    }

    if (head_1 != nullptr) {
        it->next = head_1;
        head_1->prev = it;
    }
    else if (head_2 != nullptr) {
        it->next = head_2;
        head_2->prev = it;
    }
    return dumb.next;
}

template<typename T>
constexpr singly_linked_node<T>* nth(singly_linked_node<T>* head, prelude::size_t n) {
    while (n-- > 0) {
        if (head == nullptr) {
            return nullptr;
        }
        head = head->next;
    }
    return head;
}

template<typename T>
constexpr doubly_linked_node<T>* nth(doubly_linked_node<T>* head, prelude::size_t n) {
    return reinterpret_cast<doubly_linked_node<T>*>(nth(reinterpret_cast<singly_linked_node<T>*>(head), n));
}

template<typename T>
constexpr void remove_after(singly_linked_node<T>* pos) {
    if (pos == nullptr || pos->next == nullptr) {
        return;
    }
    pos->next = pos->next->next;
}

template<typename T>
constexpr void remove_after(doubly_linked_node<T>* pos) {
    if (pos == nullptr || pos->next == nullptr) {
        return;
    }
    pos->next = pos->next->next;
    if (pos->next != nullptr) {
        pos->next->prev = pos;
    }
}

template<typename T>
constexpr void reverse(singly_linked_node<T>* head) {
    auto* curr = head;
    auto* prev = nullptr;
    while (curr != nullptr) {
        auto* tmp = curr->next;
        curr->next = prev;
        prev = curr;
        curr = tmp;
    }
    return prev;
}

template<typename T>
constexpr void reverse(doubly_linked_node<T>* head) {
    auto* curr = head;
    auto* prev = nullptr;
    while (curr != nullptr) {
        auto* tmp = curr->next;
        curr->next = prev;
        prev = curr;
        curr = tmp;
        prev->prev = curr;
    }
    return prev;
}

template<typename T>
constexpr prelude::size_t size(singly_linked_node<T>* head) {
    auto result = 0uz;
    while (head != nullptr) {
        ++result;
        head = head->next;
    }
    return result;
}

template<typename T>
constexpr prelude::size_t size(doubly_linked_node<T>* head) {
    return size(reinterpret_cast<singly_linked_node<T>*>(head));
}

template<typename T, prelude::size_t N>
constexpr prelude::size_t size(tree_node<T, N>* root) {
    if (root == nullptr) {
        return 0;
    }
    prelude::size_t result = 0;
    for (auto i = 0uz; i < N; ++i) {
        result += size(root->children[i]);
    }
    return result + 1;
}

template<typename T>
constexpr void splice_after(singly_linked_node<T>* pos, singly_linked_node<T>* head) {
    if (pos == nullptr || head == nullptr) {
        return;
    }
    auto* tmp = last(head);
    tmp->next = pos->next;
    pos->next = head;
}

template<typename T>
constexpr void splice_after(doubly_linked_node<T>* pos, doubly_linked_node<T>* head) {
    if (pos == nullptr || head == nullptr) {
        return;
    }
    auto* tmp = last(head);
    tmp->next = pos->next;
    if (pos->next != nullptr) {
        pos->next->prev = tmp;
    }
    pos->next = head;
    head->prev = pos;
}

} // namespace prelude