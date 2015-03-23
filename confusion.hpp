#ifndef CONFUSION_HPP
#define CONFUSION_HPP

#include <cinttypes>
#include <limits>
#include <utility>
#include <vector>

#include "digits.hpp"
#include "util.hpp"

template<typename P>
class Confusion: public std::vector<std::vector<uint32_t>> {
public:
    Confusion(const DigitData &data_, const std::vector<P> &perceps):
        std::vector<std::vector<uint32_t>>(DIGITS,
            std::vector<uint32_t>(DIGITS, 0))
    {
        fill(data_, perceps);
    }

    void print(FILE *stream) const {
        for (auto &row : *this) {
            for (auto cell : row)
                fprintf(stream, "%03u ", cell);

            fputc('\n', stream);
        }
    }

private:
    void fill(const DigitData &data_, const std::vector<P> &perceps) {
        for (uint32_t digit = 0; digit < DIGITS; digit += 1) {
            data_.iterExamples(digit, [&](auto x) {
                MaxTracker<uint32_t, double> maxScore;

                for (uint32_t p = 0; p < DIGITS; p += 1)
                    maxScore.consider(p, perceps[p].eval(x));

                (*this)[digit][maxScore.key] += 1;
            });
        }
    }
};

#endif