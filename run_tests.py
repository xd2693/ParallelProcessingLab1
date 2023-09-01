#!/usr/bin/env python3
import os
from subprocess import check_output
import re
from time import sleep

import matplotlib.pyplot as plt

#
#  Feel free (a.k.a. you have to) to modify this to instrument your code
#

THREADS = [0,1,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32]
LOOPS = [100000]
INPUTS = ["seq_64_test.txt","1k.txt","8k.txt","16k.txt"]

csvs = []
for inp in INPUTS:
    for loop in LOOPS:
        csv = ["{}/{}".format(inp, loop)]
        for thr in THREADS:
            inputSize = inp.split(".")[0]
            outputName = "results/result_%s_t%d.txt" % (inputSize, thr)
            cmd = "./bin/prefix_scan -o {} -n {} -i tests/{} -l {}".format(
                outputName, thr, inp, loop)
            out = check_output(cmd, shell=True).decode("ascii")
            m = re.search("time: (.*)", out)
            #print(out)
            if m is not None:
                time = m.group(1)
                csv.append(time)
            #compareName= "results/result_%s_t0.txt" % inputSize
            #cmd = "diff {} {}".format(compareName,outputName)
            #out = check_output(cmd, shell=True).decode("ascii")
            #print(out)

        csvs.append(csv)
        sleep(0.5)

header = ["microseconds"] + [str(x) for x in THREADS]

print("\n")
print(", ".join(header))
for csv in csvs:
    print (", ".join(csv))

for csv in csvs:
    plt.plot(THREADS,csv[1:],label=csv[0])

plt.xlabel("Number of threads")
plt.ylabel("Run time")
plt.legend()
plt.show()