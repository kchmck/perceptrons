#!/usr/bin/python

import sys

with open(sys.argv[1], "r") as buf:
    with open(sys.argv[2], "w") as out:
        for line in buf:
            fields = line.split(",")

            feats = [int(x) for x in fields[:64]]
            label = int(fields[64])

            out.write("{} ".format(label))

            for idx, feat in enumerate(feats):
                if feat != 0:
                    out.write("{}:{} ".format(idx, feat))

            out.write("\n")

