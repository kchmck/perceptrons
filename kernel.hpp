#ifndef KERNEL_HPP
#define KERNEL_HPP

#include <cinttypes>
#include <cmath>
#include <functional>

#include "util.hpp"
#include "vec.hpp"

typedef std::function<double(const Vec &x, const Vec &y)> KernelFn;

namespace kernel {
    // Get a polynomial kernel of the given degree.
    KernelFn poly(double d) {
        return [=](const Vec &x, const Vec &y) {
            return pow(1 + x.dot(y), d);
        };
    }

    // Get a Gaussian kernel with the given width.
    KernelFn gaussian(double width) {
        double denom = 2 * pow(width, 2);

        return [=](const Vec &x, const Vec &y) {
            return exp(-x.sub(y).norm() / denom);
        };
    }

    // Get a normalized kernel of the given kernel function.
    KernelFn normalize(const KernelFn &fn) {
        return [=](const Vec &x, const Vec &y) {
            return fn(x,y) / sqrt(fn(x,x) * fn(y,y));
        };
    }
}

#endif
