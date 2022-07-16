#pragma once

namespace prelude {

template<typename InIt, typename OutIt>
inline void copy(InIt first, InIt last, OutIt out) {
    while (first != last) {
        *out++ = *first++;
    }
}

template<typename T, typename OutIt>
inline void fill(OutIt first, OutIt last, T const& val) {
    while (first != last) {
        *first++ = val;
    }
}

template<typename OutIt, typename... Args>
inline void fill_args(OutIt first, Args&&... args) {
    (*first++ = static_cast<Args&&>(args), ...);
}

inline void fill_zeros(void* first, void* last) {
    unsigned char* ptr = static_cast<unsigned char*>(first);
    while (ptr != last) {
        *ptr++ = 0;
    }
}

template<typename T>
inline void overwrite_with_zeros(T& t) {
    void* ptr = static_cast<void*>(&t);
    prelude::fill_zeros(ptr, ptr + sizeof(T));
}


} // namespace prelude