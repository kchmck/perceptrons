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

        // Number of correct classifications and total number of examples.
        uint32_t correct = 0;
        uint32_t total = 0;

        // For each digit class in the development data, calculate the overall
        // accuracy with the current epoch.
        for (uint32_t d = 0; d < DIGITS; d += 1) {
            develData.iterExamples(d, [&](const Vec &x) {
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

        printf("%u:%u/%u\n", epochs, correct, total);
        maxAcc.consider(epochs, (double) correct / (double) total);
    }

    printf("acc:%u:%f\n", maxAcc.key, maxAcc.val);
}
