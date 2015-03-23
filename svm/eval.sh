#!/usr/bin/bash

DEGREE=4
GAMMA=0.005

run() {
    rm model-{0..9}
    rm out-{0..9}

    for dig in {0..9}; do
        ./svm_learn "$@" train-$dig model-$dig >/dev/null
    done

    for dig in {0..9}; do
        ./svm_classify test model-$dig out-$dig >/dev/null
    done

    awk '{print $1}' test >test.labels
    paste test.labels out-{0..9} >test.csv
    ./eval.py test.csv
}

echo polynomial
run -t 1 -r 1 -d $DEGREE

echo gaussian
run -t 2 -g $GAMMA
