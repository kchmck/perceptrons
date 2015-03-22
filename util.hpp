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
        key(),
        val(std::numeric_limits<Val>::min())
    {}

    // Possibly update the maximum value with the given values.
    void consider(Key k, Val v) {
        if (v > val) {
            key = k;
            val = v;
        }
    }
};

template<typename Percep>
class AccTester {
private:
    const std::vector<Percep> &perceps;
    const DigitData &data;

public:
    uint32_t correct, total;

public:
    AccTester(const std::vector<Percep> &perceps_, const DigitData &data_):
        perceps(perceps_),
        data(data_),
        correct(0),
        total(0)
    {
        run();
    }

    inline double acc() const { return (double) correct / (double) total; }

private:
    void run() {
        // For each digit class in the development data, calculate the overall
        // accuracy with the current epoch.
        for (uint32_t d = 0; d < DIGITS; d += 1) {
            data.iterExamples(d, [&](const Vec &x) {
                // Tracks the maximum score over the 10 perceptrons.
                MaxTracker<uint32_t, double> maxScore;

                // Evaluate each perceptron.
                for (uint32_t p = 0; p < DIGITS; p += 1)
                    maxScore.consider(p, perceps[p].eval(x));

                // The example was classified correctly if the perceptron with
                // the maximum score was the one trained for the current digit
                // class.
                if (maxScore.key == d)
                    correct += 1;

                total += 1;
            });
        }
    }
};

#endif
