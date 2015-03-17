#include <fstream>

#include "digits.hpp"
#include "perceptron.hpp"
#include "util.hpp"
#include "vec.hpp"

static constexpr uint32_t EPOCHS[] = {1, 5, 10, 20};

int main() {
    std::fstream trainStream("train.dat");
    const DigitData trainData(trainStream);

    std::fstream develStream("devel.dat");
    const DigitData develData(develStream);

    MaxTracker<uint32_t, double> maxAcc;

    for (auto epochs : EPOCHS) {
        std::vector<perceptron::Basic> perceps;

        trainData.iterDigits([&](uint32_t d, const Phi &phi, const Labels &labels) {
            (void) d;
            perceps.emplace_back(phi, labels, epochs);
        });

        uint32_t correct = 0;
        uint32_t total = 0;

        develData.iterDigits([&](uint32_t d, const Phi &phi, const Labels &labels) {
            (void) labels;

            for (size_t i = 0; i < phi.size(); i += 1) {
                if (labels[i] < 0)
                    continue;

                MaxTracker<uint32_t, double> maxScore;

                for (uint32_t p = 0; p < DIGITS; p += 1)
                    maxScore.consider(p, perceps[p].eval(phi[i]));

                if (maxScore.key == d)
                    correct += 1;

                total += 1;
            }
        });

        printf("%u:%u/%u\n", epochs, correct, total);
        maxAcc.consider(epochs, (double) correct / (double) total);
    }

    printf("acc:%u:%f\n", maxAcc.key, maxAcc.val);
}
