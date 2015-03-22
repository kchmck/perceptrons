#!/usr/bin/python

import sys

correct = 0
total = 0
con = []

for r in range(10):
    con.append([0] * 10)

with open(sys.argv[1], "r") as buf:
    for line in buf:
        fields = line.split()

        label = int(fields[0])
        results = [float(x) for x in fields[1:]]

        m = max(results)
        i = results.index(m)

        con[label][i] += 1

        if i == label:
            correct += 1

        total += 1

print("{}/{} ({}%)".format(correct, total, correct / total))

for row in con:
    for col in row:
        print("{:03} ".format(col), end="")

    print("")
