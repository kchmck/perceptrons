#include <cstdio>
#include <fstream>

#include "confusion.hpp"
#include "digits.hpp"
#include "perceptron.hpp"
#include "util.hpp"
#include "vec.hpp"

// Epochs to test.
static constexpr uint32_t EPOCHS[] = {1, 5, 10, 20};

int main() {
    std::fstream trainStream("train.dat");
    const DigitData trainData(trainStream);

    std::fstream develStream("devel.dat");
    const DigitData develData(develStream);

    // Tracks the maximum accuracy over all epochs.
    MaxTracker<uint32_t, double> maxAcc;

    for (auto epochs : EPOCHS) {
        // Stores the perceptrons trained on each digit for the current epoch.
        std::vector<perceptron::Linear> perceps;

        // Train a perceptron for each digit.
        trainData.iterDigits([&](auto d, auto &phi, auto &labels) {
            (void) d;
            perceps.emplace_back(phi, labels, epochs);
        });

        const Confusion<perceptron::Linear> c(develData, perceps);

        printf("%u:%u/%u (%f)\n", epochs, c.correct, c.total, c.acc());
        maxAcc.consider(epochs, c.acc());
    }

    printf("acc:%u:%f\n", maxAcc.key, maxAcc.val);
}
