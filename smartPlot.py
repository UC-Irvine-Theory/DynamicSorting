import string
import sys

import numpy as np
import matplotlib.pyplot as plt

from aggregateResult import loadData, getPlots, plotInversions

path = sys.argv[1]

print "Loading..."
results = loadData(path)

print "Ready!\n"
while True:
    line = raw_input("Input what you want to plot 'n ,alg ,startConfig ,swapType ,numSwaps' or q to quit\n")

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

