#ifndef UTIL_HPP
#define UTIL_HPP

static inline int sgn(double x) {
    return (int) copysign(1.0, x);
}

#endif
