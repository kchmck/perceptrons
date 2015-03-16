#ifndef DIGITS_HPP
#define DIGITS_HPP

#include <cassert>
#include <fstream>
#include <vector>

class DigitData {
private:
    static constexpr size_t FEATURES = 64;
    static constexpr uint32_t DIGITS = 10;

    typedef std::function<void(uint32_t, const Phi &, const Labels &)> IterFn;

public:
    Phi phi;
    std::vector<Labels> digitLabels;

public:
    DigitData(std::istream &stream):
        phi(),
        digitLabels(DIGITS)
    {
        load(stream);
    }

    void iterDigits(const IterFn &fn) const {
        for (uint32_t digit = 0; digit < DIGITS; digit += 1)
            fn(digit, phi, digitLabels[digit]);
    }

private:
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

            for (uint32_t digit = 0; digit < DIGITS; digit += 1)
                digitLabels[digit].push_back(label == digit ? +1 : -1);
        }
    }
};

#endif
