#ifndef NDEBUG

#include <cstddef>

void *operator new (std::size_t sz, const char *file, int line);

void *operator new[] (std::size_t sz, const char *file, int line);

void operator delete (void *p) noexcept;

void operator delete[] (void *p) noexcept;

void operator delete (void *p, std::size_t n) noexcept;

void operator delete[] (void *p, std::size_t n) noexcept;

#define new new (__FILE__, __LINE__)

#endif // NDEBUG
