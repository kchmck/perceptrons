#ifndef TIMER_HPP
#define TIMER_HPP

#include <cassert>
#include <chrono>
#include <cinttypes>
#include <functional>

// Accumulates the time a function takes to execute.
class Timer {
public:
    typedef std::function<void()> TimeFn;

public:
    // Time in nanoseconds.
    uintmax_t total;

public:
    Timer():
        total(0)
    {}

    void add(const TimeFn &fn) {
        auto before = std::chrono::steady_clock::now();
        fn();
        auto after = std::chrono::steady_clock::now();

        uintmax_t prev = total;
        total += (after - before).count();

        // Check for overflow.
        assert(total >= prev);
    }
};

#endif
