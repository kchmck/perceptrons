#include <fstream>

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
        std::vector<perceptron::Basic> perceps;

        // Train a perceptron for each digit.
        trainData.iterDigits([&](uint32_t d, const Phi &phi, const Labels &labels) {
            (void) d;
            perceps.emplace_back(phi, labels, epochs);
        });

        const AccTester<perceptron::Basic> t(perceps, develData);

        printf("%u:%u/%u (%f)\n", epochs, t.correct, t.total, t.acc());
        maxAcc.consider(epochs, t.acc());
    }

    printf("acc:%u:%f\n", maxAcc.key, maxAcc.val);
}
