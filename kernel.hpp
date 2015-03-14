#ifndef KERNEL_HPP
#define KERNEL_HPP

#include <cinttypes>
#include <cmath>
#include <functional>

#include "vec.hpp"

typedef std::function<double(size_t i, size_t j)> KernelFn;

namespace kernel {
    KernelFn poly(const Phi &phi, double d) {
        return [&](size_t i, size_t j) {
            return pow(phi[i].dot(phi[j]), d);
        };
    }

    KernelFn gaussian(const Phi &phi, double width) {
        double denom = 2 * pow(width, 2);

        return [&](size_t i, size_t j) {
            return exp(-phi[i].sub(phi[j]).norm() / denom);
        };
    }

    KernelFn normalize(const KernelFn &fn) {
        return [&](size_t i, size_t j) {
            return fn(i,j) / sqrt(fn(i,i) * fn(j,j));
        };
    }
}

#endif
