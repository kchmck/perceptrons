#ifndef KERNEL_HPP
#define KERNEL_HPP

#include <cinttypes>
#include <cmath>
#include <functional>
#include <vector>

#include "util.hpp"
#include "vec.hpp"

typedef std::function<double(const Vec &x, const Vec &y)> KernelFn;

class CachedKernel {
private:
    const KernelFn fn;
    const size_t n;
    std::vector<double> cache;

public:
    CachedKernel(const Phi &phi, const KernelFn fn_):
        fn(fn_),
        n(phi.size()),
        cache(n * n)
    {
        build(phi);
    }

    inline double operator()(size_t xi, size_t yi) const {
        return cache.at(xi * n + yi);
    }

    inline double operator()(const Vec &x, const Vec &y) const {
        return fn(x, y);
    }

private:
    void build(const Phi &phi) {
        for (size_t i = 0; i < phi.size(); i += 1)
            for (size_t j = 0; j < phi.size(); j += 1)
                cache.at(i * n + j) = fn(phi[i], phi[j]);
    }
};

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
