#!/usr/bin/env python3
import os
from subprocess import check_output
import re
from time import sleep
import matplotlib

matplotlib.use('Agg')
import matplotlib.pyplot as plt

#
#  Feel free (a.k.a. you have to) to modify this to instrument your code
#

THREADS = [0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32]
#THREADS = [0]
LOOPS = [10]
INPUTS = ["8k.txt"]

csvs = []
for inp in INPUTS:
    for loop in LOOPS:
        csv = ["{}/{}".format(inp, loop)]
        for thr in THREADS:
            inputSize = inp.split(".")[0]
            outputName = "results/result_%s_t%d.txt" % (inputSize, thr)
            cmd = "./bin/prefix_scan -o {} -n {} -i tests/{} -l {} -s".format(
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


plt.ticklabel_format(style='sci',scilimits=(0,0),axis='y')
for csv in csvs:
    plt.plot(THREADS, list(map(int,csv[1:])), 'o-', label = csv[0])

plt.xlabel("Number of threads")
plt.ylabel("Run time in microseconds")
plt.legend()
plt.savefig('10loop8k_s.png')