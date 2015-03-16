#ifndef UTIL_HPP
#define UTIL_HPP

#include <cmath>
#include <limits>

static inline int sgn(double x) {
    return (int) copysign(1.0, x);
}

// This started out as a std::pair, but it became repeated enough to warrant its
// own class. I tried to subclass pair, but its assignment operator wasn't
// working.
template<typename Key, typename Val>
class MaxTracker {
public:
    Key key;
    Val val;

public:
    MaxTracker():
        key(0),
        val(std::numeric_limits<Val>::min())
    {}

    void consider(Key k, Val v) {
        if (v > val) {
            key = k;
            val = v;
        }
    }
};

#endif
