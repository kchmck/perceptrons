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
    std::vector<uint32_t> labels;

public:
    DigitData(std::istream &stream):
        phi(),
        labels()
    {
        load(stream);
    }

    void iterDigits(const IterFn &fn) const {
        Labels binaryLabels(labels.size(), 0);

        for (uint32_t digit = 0; digit < DIGITS; digit += 1) {
            genLabels(binaryLabels, digit);
            fn(digit, phi, binaryLabels);
        }
    }

private:
    void genLabels(Labels &binaryLabels, uint32_t digit) const {
        for (size_t i = 0; i < labels.size(); i += 1)
            binaryLabels[i] = labels[i] == digit ? +1 : -1;
    }

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

            uint32_t label;
            assert(stream >> label);

            phi.emplace_back(std::move(x));
            labels.push_back(label);
        }
    }
};

#endif
