#ifndef CONFUSION_HPP
#define CONFUSION_HPP

#include <cinttypes>
#include <utility>
#include <vector>

#include "digits.hpp"
#include "util.hpp"

// A confusion matrix C where C[i,j] represents the number of times digit j was
// predicted where the actual digit was i.
template<typename P>
class Confusion: public std::vector<std::vector<uint32_t>> {
public:
    // The number of correct guesses and the total number of tests.
    uint32_t correct, total;

public:
    Confusion(const DigitData &data_, const std::vector<P> &perceps):
        std::vector<std::vector<uint32_t>>(DIGITS,
            std::vector<uint32_t>(DIGITS, 0)),
        correct(0),
        total(0)
    {
        fill(data_, perceps);
    }

    // Get the overall accuracy.
    inline double acc() const { return (double) correct / (double) total; }

private:
    // Fill in the matrix by running the given perceptrons on the given test
    // examples.
    void fill(const DigitData &data_, const std::vector<P> &perceps) {
        assert(perceps.size() == DIGITS);

        // For each digit class, iterate over the examples of that class.
        for (uint32_t digit = 0; digit < DIGITS; digit += 1) {
            data_.iterExamples(digit, [&](auto &x) {
                // Tracks the maximum score over the perceptrons and what digit
                // it's associated with.
                MaxTracker<uint32_t, double> maxScore;

                // Evaluate all the perceptrons.
                for (uint32_t p = 0; p < DIGITS; p += 1)
                    maxScore.consider(p, perceps[p].eval(x));

                // If the perceptron with the maximum score is associated with
                // the current digit, consider the guess correct.
                if (maxScore.key == digit)
                    correct += 1;

                total += 1;

                (*this)[digit][maxScore.key] += 1;
            });
        }
    }
};

#endif
