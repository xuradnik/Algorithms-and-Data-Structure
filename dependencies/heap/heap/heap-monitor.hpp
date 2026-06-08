#ifndef HEAP_HEAP_MONITOR_HPP
#define HEAP_HEAP_MONITOR_HPP

#ifndef NDEBUG

#include <cstdint>
#include <map>
#include <string>

namespace heap {

class HeapMonitor {
public:
    HeapMonitor(const HeapMonitor&) = delete;
    void operator= (const HeapMonitor&) = delete;
    HeapMonitor(HeapMonitor&&) = delete;
    void operator= (HeapMonitor&&) = delete;

    static HeapMonitor &getInstance() {
        static HeapMonitor instance;
        return instance;
    }

public:
    ~HeapMonitor();
    void logAllocation(void *p, std::string file, int line);
    void logDeletion(void *p);

private:
    HeapMonitor() = default;

private:
    std::map<std::intptr_t, std::string> m_allocations{};
    bool m_destructor_running{false};
};

} // namespace heap


#include <heap/header-begin.hpp>


#endif // NDEBUG

#endif // HEAP-MONITOR_HPP
