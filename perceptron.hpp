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
    // An abstract base class for perceptron common stuff.
    class Base {
    protected:
        // The associated examples and labels.
        const Phi &phi;
        const Labels &labels;
        // The maximum number of epochs.
        const uint32_t epochs;

    protected:
        Base(const Phi &phi_, const Labels &labels_, uint32_t epochs_):
            phi(phi_),
            labels(labels_),
            epochs(epochs_)
        {
            assert(phi.size() == labels.size());
        }

        // Perform the main perceptron loop for the maximum number of epochs.
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

        // Perform the inner function of the perceptron loop.
        virtual bool inner(size_t i) = 0;
        // Evaluate the example with the learned parameters.
        virtual double eval(const Vec &x) const = 0;
        // Do any final processing on the learned parameters.
        virtual void finish() {}

    public:
        // Run the perceptron.
        void run() {
            loop();
            finish();
        }
    };

    // Handles the averaging part of the averaged perceptron.
    class Averager {
    private:
        typedef std::function<bool()> InnerFn;

    public:
        // The average accumulator.
        Vec avg;

    private:
        // The average divisor.
        uint32_t counter;

    public:
        Averager(size_t n):
            avg(n, 0.0),
            counter(0)
        {}

        // Run the inner part of the averaged perceptron loop, wrapping the
        // given function and accumulating the given vector.
        bool handle(const Vec &accum, const InnerFn &innerFn) {
            bool converged = innerFn();

            counter += 1;
            avg.add_in(accum);

            return converged;
        }

        // Finish processing the average vector.
        void finish() {
            avg.div_in((double) counter);
        }

        // Normalize the average vector.
        void normalize() {
            avg.normalize();
        }
    };

    // The most basic perceptron.
    class Linear: public Base {
    public:
        Vec weights;

    public:
        Linear(const Phi &phi_, const Labels &labels_, uint32_t epochs_):
            Base(phi_, labels_, epochs_),
            weights(phi[0].size(), 0.0)
        {
            run();
        }

        double eval(const Vec &x) const override {
            return weights.dot(x);
        }

        // Normalize the weights vector.
        void finish() override { weights.normalize(); }

    // HACK: there's a bug in gcc (#58972) that lambdas can't access
    // private/protected members, so this has to be public.
    public:
        bool inner(size_t i) override {
            if (sgn(weights.dot(phi[i])) != labels[i]) {
                weights.add_in(phi[i].mult(labels[i]));
                return false;
            }

            return true;
        }
    };

    // The averaged perceptron.
    class Averaged: public Linear {
    protected:
        Averager averager;

    public:
        Averaged(const Phi &phi_, const Labels &labels_, uint32_t epochs_):
            Linear(phi_, labels_, epochs_),
            averager(phi[0].size())
        {
            run();
        }

        void finish() override {
            averager.finish();
            averager.normalize();
        }

        double eval(const Vec &x) const override {
            return averager.avg.dot(x);
        }

    protected:
        bool inner(size_t i) override {
            return averager.handle(weights, [=] {
                return Linear::inner(i);
            });
        }
    };

    // The normal kernel perceptron.
    class Kernel: public Base {
    protected:
        typedef std::function<void(double)> SupportIterFn;

        const CachedKernel &fn;

    public:
        Vec alphas;

    public:
        Kernel(const Phi &phi_, const Labels &labels_, uint32_t epochs_,
               const CachedKernel &fn_):
            Base(phi_, labels_, epochs_),
            fn(fn_),
            alphas(phi.size(), 0.0)
        {
            run();
        }

        double eval(const Vec &x) const override {
            return eval(alphas, x);
        }

        void iterSupport(const SupportIterFn &sfn) const {
            for (auto a : alphas)
                // BAD: exact float comparison.
                if (a != 0.0)
                    sfn(a);
        }

    // HACK: same as above.
    public:
        bool inner(size_t i) override {
            if (sgn(eval_cached(alphas, i)) != labels[i]) {
                alphas[i] += 1;
                return false;
            }

            return true;
        }

    protected:
        // Evaluate the given feature vector with the given alphas using the
        // current examples and labels.
        double eval(const Vec &alphas_, const Vec &x) const {
            double sum = 0.0;

            for (size_t j = 0; j < phi.size(); j += 1)
                sum += (double) alphas_[j] * (double) labels[j] * fn(x, phi[j]);

            return sum;
        }


        double eval_cached(const Vec &alphas_, size_t i) const {
            double sum = 0.0;

            for (size_t j = 0; j < phi.size(); j += 1)
                sum += (double) alphas_[j] * (double) labels[j] * fn(i, j);

            return sum;
        }
    };

    // The averaged kernel perceptron.
    class AveragedKernel: public Kernel {
    protected:
        Averager averager;

    public:
        AveragedKernel(const Phi &phi_, const Labels &labels_, uint32_t epochs_,
                       const CachedKernel &fn_):
            Kernel(phi_, labels_, epochs_, fn_),
            averager(phi.size())
        {
            run();
        }

        void finish() override { averager.finish(); }

        double eval(const Vec &x) const override {
            return Kernel::eval(averager.avg, x);
        }

        void iterSupport(const SupportIterFn &sfn) const {
            for (auto a : averager.avg)
                if (a != 0.0)
                    sfn(a);
        }

    protected:
        bool inner(size_t i) override {
            return averager.handle(alphas, [=] {
                return Kernel::inner(i);
            });
        }
    };
}

#endif
