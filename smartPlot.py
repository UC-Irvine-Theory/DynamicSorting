import string
import sys

import numpy as np
import matplotlib.pyplot as plt

from aggregateResult import loadData, getPlots, plotInversions, printOptions

if len(sys.argv) != 2:
    print "Too few or too many arguments, only put the directory of the results"
    sys.exit()

path = sys.argv[1]

print "Loading..."
results, options = loadData(path)

print "Ready!\n"
print "Input your options at the prompt."
print "\tFor a single option put the name eg: BUBBLE"
print "\tFor a subset separate by -       eg: INSERTION-QUICK"
print "\tFor all the options put *        eg: *"
while True:

    printOptions(options)

    line = raw_input("Input what you want to plot '<n> <alg> <startConfig> <swapType> <numSwaps>' or q to quit\n")

    if line == "q":
        break

    try:
        n,alg,startConfig,swapType,numSwaps = string.split(line, " ")
    except:
        print "Couldn't split:  " + line 
        continue

    alg         = alg.upper()
    startConfig = startConfig.upper()
    swapType    = swapType.upper()

    print "\tn: " + n + "\n\talg: " + alg + "\n\tstartConfig: " + startConfig
    print "\tswapType: " + swapType + "\n\tnumSwaps: " + numSwaps

    try:
        plots = getPlots([], results, [n,alg,startConfig,swapType,numSwaps], "")
    except:
        print "Problem with the params you provided:"
        print "\tn: " + n + "\n\talg: " + alg + "\n\tstartConfig: " + startConfig
        print "\tswapType: " + swapType + "\n\tnumSwaps: " + numSwaps
        continue

    plt.title(line)
    plotInversions(plots)
