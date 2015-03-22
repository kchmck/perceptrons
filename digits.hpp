#ifndef DIGITS_HPP
#define DIGITS_HPP

#include <cassert>
#include <fstream>
#include <functional>
#include <vector>

#include "vec.hpp"

// Just the number of digits from 0 to 9.
static constexpr uint32_t DIGITS = 10;

// Loads the data in the format of this project and makes it iterable.
class DigitData {
private:
    // The number of features per example.
    static constexpr size_t FEATURES = 64;

    typedef std::function<void(uint32_t, const Phi &, const Labels &)> DigitIterFn;
    typedef std::function<void(const Vec &)> ExampleIterFn;

public:
    // The current set of examples.
    Phi phi;
    // The set of labels associated with each digit class.
    std::vector<Labels> digitLabels;

public:
    DigitData(std::istream &stream):
        phi(),
        digitLabels(DIGITS)
    {
        load(stream);
    }

    // Iterate over each digit class and call the given function with the set of
    // examples and the labels for that digit class.
    void iterDigits(const DigitIterFn &fn) const {
        // For each digit, call the function with the corresponding labels
        // vector.
        for (uint32_t digit = 0; digit < DIGITS; digit += 1)
            fn(digit, phi, digitLabels[digit]);
    }

    // Iterate over all the examples in the given digit class and call the given
    // function on each feature vector.
    void iterExamples(uint32_t digit, const ExampleIterFn &fn) const {
        for (size_t i = 0; i < phi.size(); i += 1)
            if (digitLabels[digit][i] > 0)
                fn(phi[i]);
    }

private:
    // Load the features and labels from the given stream.
    void load(std::istream &stream) {
        for (;;) {
            Vec x(FEATURES, 0.0);

            for (size_t i = 0; i < FEATURES; i += 1) {
                double num;
                stream >> num;

                if (stream.eof())
                    return;

                assert(stream);

                x[i] = num;

                char next;
                assert(stream.get(next));
                assert(next == ',');
            }

            phi.emplace_back(std::move(x));

            uint32_t label;
            assert(stream >> label);

            // For each set of labels, add the current example as a positive if
            // the label matches and as a negative if it doesn't.
            for (uint32_t digit = 0; digit < DIGITS; digit += 1)
                digitLabels[digit].push_back(label == digit ? +1 : -1);
        }
    }
};

#endif
