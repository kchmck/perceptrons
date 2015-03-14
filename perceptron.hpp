#ifndef PERCEPTRON_HPP
#define PERCEPTRON_HPP

#include <cmath>
#include <cinttypes>

#include "kernel.hpp"
#include "vec.hpp"

static int sgn(double x) {
    return (int) copysign(1.0, x);
}

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

        void run() {
            for (uint32_t epoch = 0; epoch < epochs; epoch += 1) {
                bool converged = true;

                for (size_t i = 0; i < phi.size(); i += 1)
                    converged = inner(i);

                if (converged)
                    break;
            }
        }

        virtual bool inner(size_t i) = 0;
    };

    class AbstractAveraged: public Abstract {
    private:
        uint32_t counter;

    public:
        Vec avg;

    public:
        AbstractAveraged(const Phi &phi_, const Labels &labels_, uint32_t epochs_):
            Abstract(phi_, labels_, epochs_),
            counter(0),
            avg(phi[0].size(), 0.0)
        {}

    protected:
        void run() {
            Abstract::run();
            avg.div_in((double) counter);
        }

        bool inner(size_t i) {
            bool converged = innerinner(i);

            average();
            counter += 1;

            return converged;
        }

        virtual bool innerinner(size_t i) = 0;
        virtual void average() = 0;
    };

    class Basic: public Abstract {
    public:
        Vec weights;

    public:
        Basic(const Phi &phi_, const Labels &labels_, uint32_t epochs_):
            Abstract(phi_, labels_, epochs_),
            weights(phi[0].size(), 0.0)
        {
            run();
        }

    private:
        bool inner(size_t i) {
            if (sgn(weights.dot(phi[i])) != labels[i]) {
                weights.add_in(phi[i].mult(labels[i]));
                return false;
            }

            return true;
        }
    };

    class Averaged: public AbstractAveraged {
    private:
        Vec weights;

    public:
        Averaged(const Phi &phi_, const Labels &labels_, uint32_t epochs_):
            AbstractAveraged(phi_, labels_, epochs_),
            weights(phi[0].size(), 0.0)
        {
            run();
        }

        bool innerinner(size_t i) {
            if (sgn(weights.dot(phi[i])) != labels[i]) {
                weights.add_in(phi[i].mult(labels[i]));
                return false;
            }

            return true;
        }

        void average() {
            avg.add_in(weights);
        }
    };

    class KernelCalculator {
    private:
        const KernelFn &fn;

    public:
        Vec alphas;

    public:
        KernelCalculator(const KernelFn &fn_, size_t n):
            fn(fn_),
            alphas(n, 0)
        {}

        double calc(const Phi &phi, const Labels &labels, size_t i) {
            double accum = 0.0;

            for (size_t j = 0; j < phi.size(); j += 1)
                accum += alphas[j] * (double) labels[j] * fn(i, j);

            return accum;
        }
    };

    class Kernel: public Abstract {
    private:
        const KernelCalculator kernel;

    public:
        Kernel(const Phi &phi_, const Labels &labels_, uint32_t epochs_,
               const KernelFn &fn_):
            Abstract(phi_, labels_, epochs_),
            fn(fn_),
            alphas(phi.size(), 0)
        {
            run();
        }

    private:
        double calc_sum(size_t i) {
            double sum = 0.0;

            for (size_t j = 0; j < phi.size(); j += 1)
                sum += (double) alphas[j] * (double) labels[j] * fn(i, j);

            return sum;
        }

        bool inner(size_t i) {
            if (sgn(calc_sum(i)) != labels[i]) {
                alphas[i] += 1;
                return false;
            }

            return true;
        }
    };

    class AveragedKernel: public Abstract {
    private:
        const KernelCalculator kernel;

    public:
        AveragedKernel(const Phi &phi_, const Labels &labels_, uint32_t epochs_,
                       const KernelFn &fn_):
            Abstract(phi_, labels_, epochs_),
            kernel(fn_, phi[0].size())
        {
            run();
        }

        bool innerinner(size_t i) {
        }

        void average() {
            avg.add_in(alphas);
        }
    };
}

#endif
