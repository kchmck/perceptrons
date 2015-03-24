#ifndef KERNEL_HPP
#define KERNEL_HPP

#include <cinttypes>
#include <cmath>
#include <functional>
#include <vector>

#include "util.hpp"
#include "vec.hpp"

// A basic kernel function
typedef std::function<double(const Vec &x, const Vec &y)> KernelFn;

// A kernel function with precomputed results.
class CachedKernel {
private:
    // The kernel function to use.
    const KernelFn fn;
    // The number of examples to precompute results pairwise.
    const size_t n;
    // The precomputed results themselves. This is an emulated 2D vector to
    // avoid the indirection of nested vectors (since its used in tight loops.)
    std::vector<double> cache;

public:
    CachedKernel(const Phi &phi, const KernelFn fn_):
        fn(fn_),
        n(phi.size()),
        // Who cares about symmetry? #yolo
        cache(n * n)
    {
        build(phi);
    }

    // Get the precomputed results between the two examples, as indexes into
    // phi.
    inline double operator()(size_t xi, size_t yi) const {
        return cache.at(xi * n + yi);
    }

    // Run the kernel function on the two examples.
    inline double operator()(const Vec &x, const Vec &y) const {
        return fn(x, y);
    }

private:
    // Fill in the cache table.
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
            return pow(x.dot(y), d);
        };
    }

    // Get a Gaussian kernel with the given width.
    KernelFn gaussian(double width) {
        double denom = -2.0 * pow(width, 2.0);

        return [=](const Vec &x, const Vec &y) {
            return exp(x.sub(y).normSquared() / denom);
        };
    }

    // Get a normalized kernel of the given kernel function.
    KernelFn normalize(const KernelFn fn) {
        return [=](const Vec &x, const Vec &y) {
            return fn(x,y) / sqrt(fn(x,x) * fn(y,y));
        };
    }
}

#endif
