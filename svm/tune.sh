#!/usr/bin/bash

run() {
    rm model-{0..9}
    rm out-{0..9}

    for dig in {0..9}; do
        ./svm_learn "$@" train-$dig model-$dig >/dev/null
    done

    for dig in {0..9}; do
        ./svm_classify devel model-$dig out-$dig >/dev/null
    done

    awk '{print $1}' devel >devel.labels
    paste devel.labels out-{0..9} >devel.csv
    ./eval.py devel.csv
}

for deg in 2 3 4 5 6; do
    echo degree $deg
    run -t 1 -r 1 -d $deg
done

echo

for gamma in 2 0.125 0.05555556 0.02 0.005; do
    echo gamma $gamma
    run -t 2 -g $gamma
done
