#!/usr/bin/python

import sys

with open(sys.argv[1], "r") as buf:
    for line in buf:
        for dig in range(10):
            with open("{}-{}".format(sys.argv[2], dig), "a") as out:
                fields = line.split(",")

                feats = [int(x) for x in fields[:64]]
                label = int(fields[64])

                if label == dig:
                    out.write("1 ")
                else:
                    out.write("-1 ")

                for idx, feat in enumerate(feats):
                    if feat != 0:
                        out.write("{}:{} ".format(idx, feat))

                out.write("\n")
