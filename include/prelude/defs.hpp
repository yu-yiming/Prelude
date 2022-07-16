#pragma once

#ifdef _WIN32
#define WINDOWS 1
#define OSX 0
#define LINUX 0
#elif defined(__APPLE__)
#define WINDOWS 0
#define OSX 1
#define LINUX 0
#elif defined(__linux__)
#define WINDOWS 0
#define OSX 0
#define LINUX 1
#endif

namespace prelude {


using size_t = unsigned long long;
using ssize_t = long long;


// Stack allocation
#if WINDOWS
template<typename T>
T* stackalloc(size_t n) {
    return static_cast<T*>(_malloca(n * sizeof(T)));
}
#elif OSX || LINUX
template<typename T>
T* stackalloc(size_t n) {
    return static_cast<T*>(alloca(n * sizeof(T)));
}
#endif

} // namespace prelude
