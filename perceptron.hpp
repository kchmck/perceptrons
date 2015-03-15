#ifndef PERCEPTRON_HPP
#define PERCEPTRON_HPP

#include <cinttypes>
#include <cmath>
#include <functional>
#include <utility>

#include "kernel.hpp"
#include "util.hpp"
#include "vec.hpp"

namespace perceptron {
    class Abstract {
    protected:
        const Phi &phi;
        const Labels &labels;
        const uint32_t epochs;

    protected:
        Abstract(const Phi &phi_, const Labels &labels_, uint32_t epochs_):
            phi(phi_),
            labels(labels_),
            epochs(epochs_)
        {
            assert(phi.size() == labels.size());
        }

        void loop() {
            for (uint32_t epoch = 0; epoch < epochs; epoch += 1) {
                bool converged = true;

                for (size_t i = 0; i < phi.size(); i += 1)
                    if (!inner(i))
                        converged = false;

                if (converged)
                    break;
            }
        }

        virtual bool inner(size_t i) = 0;
        virtual Vec finish() = 0;

    public:
        Vec run() {
            loop();
            return finish();
        }
    };

    class Averager {
    private:
        typedef std::function<bool()> InnerFn;

    public:
        Vec avg;

    private:
        uint32_t counter;

    public:
        Averager(size_t n):
            avg(n, 0.0),
            counter(0)
        {}

        bool handle(const Vec &accum, const InnerFn &innerFn) {
            bool converged = innerFn();

            counter += 1;
            avg.add_in(accum);

            return converged;
        }

        Vec finish() {
            avg.div_in((double) counter);
            return std::move(avg);
        }
    };

    class Basic: public Abstract {
    protected:
        Vec weights;

    public:
        Basic(const Phi &phi_, const Labels &labels_, uint32_t epochs_):
            Abstract(phi_, labels_, epochs_),
            weights(phi[0].size(), 0.0)
        {}

        Vec finish() { return std::move(weights); }

    // HACK: there's a bug in gcc (#58972) that lambdas can't access
    // private/protected members, so this has to be public.
    public:
        bool inner(size_t i) {
            if (sgn(weights.dot(phi[i])) != labels[i]) {
                weights.add_in(phi[i].mult(labels[i]));
                return false;
            }

            return true;
        }
    };

    class Averaged: public Basic {
    protected:
        Averager averager;

    public:
        Averaged(const Phi &phi_, const Labels &labels_, uint32_t epochs_):
            Basic(phi_, labels_, epochs_),
            averager(phi[0].size())
        {}

        Vec finish() { return averager.finish(); }

    protected:
        bool inner(size_t i) {
            return averager.handle(weights, [&] {
                return Basic::inner(i);
            });
        }
    };

    class Kernel: public Abstract {
    protected:
        const KernelFn &fn;
        Vec alphas;

    public:
        Kernel(const Phi &phi_, const Labels &labels_, uint32_t epochs_,
               const KernelFn &fn_):
            Abstract(phi_, labels_, epochs_),
            fn(fn_),
            alphas(phi.size(), 0)
        {}

        Vec finish() { return std::move(alphas); }

    public:
        bool inner(size_t i) {
            if (sgn(kernel::eval(fn, phi, labels, alphas, phi[i])) != labels[i]) {
                alphas[i] += 1;
                return false;
            }

            return true;
        }
    };

    class AveragedKernel: public Kernel {
    protected:
        Averager averager;

    public:
        AveragedKernel(const Phi &phi_, const Labels &labels_, uint32_t epochs_,
                       const KernelFn &fn_):
            Kernel(phi_, labels_, epochs_, fn_),
            averager(phi.size())
        {}

        Vec finish() { return averager.finish(); }

    protected:
        bool inner(size_t i) {
            return averager.handle(alphas, [&] {
                return Kernel::inner(i);
            });
        }
    };
}

#endif
