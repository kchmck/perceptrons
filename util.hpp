#ifndef UTIL_HPP
#define UTIL_HPP

#include <cinttypes>
#include <cmath>
#include <limits>
#include <vector>

#include "digits.hpp"
#include "vec.hpp"

// Calculate f(x) = +1 if x >= 0 and f(x) = -1 if x < 0 (or x = -0).
static inline int sgn(double x) {
    return (int) copysign(1.0, x);
}

// Tracks a maximum value and associates it with the "key" that led to it. This
// started out as a std::pair, but it became repeated enough to warrant its own
// class. I tried to subclass pair, but its assignment operator wasn't working.
template<typename Key, typename Val>
class MaxTracker {
public:
    // The associated key.
    Key key;
    // The maximum value.
    Val val;

public:
    MaxTracker():
        // At least something will blow up this way.
        key(std::numeric_limits<Key>::max()),
        val(-std::numeric_limits<Val>::max())
    {}

    // Possibly update the maximum value with the given values.
    void consider(Key k, Val v) {
        if (v > val) {
            key = k;
            val = v;
        }
    }
};

#endif
