import string

from os import listdir
import os.path 

import collections

import numpy as np
import matplotlib.pyplot as plt


colors = {'QUICK': 'cyan', 'BLOCKQUICK': 'blue', 'BUBBLE':'red', 'COCKTAIL' : 'green', 'INSERTION': 'magenta'}

def loadData(path):
#COCKTAIL-1000-REVERSE-HOTSPOT-16-14.out
    raw = {}
    for f in listdir(path):
        if not f.endswith(".out"):
            continue
        basename = os.path.basename(f)
        name,ext = os.path.splitext(basename)

        alg,size,config,swapType,numSwaps,seed = string.split(name, '-')

        if not size in raw:
            raw[size] = {}
        if not alg in raw[size]:
            raw[size][alg] = {}
        if not config in raw[size][alg]:
            raw[size][alg][config] = {}
        if not swapType in raw[size][alg][config]:
            raw[size][alg][config][swapType] = {}
        if not numSwaps in raw[size][alg][config][swapType]:
            raw[size][alg][config][swapType][numSwaps] = {}
            if not seed in raw[size][alg][config][swapType][numSwaps]:
                #maxRows = int(size) * 10
                #raw[size][alg][config][swapType][numSwaps][seed] = np.genfromtxt(path+f, delimiter=",", max_rows=maxRows)
                raw[size][alg][config][swapType][numSwaps][seed] = np.genfromtxt(path+f, delimiter=",")

#size -> alg -> config
    results = {}

    for size, algs in raw.items():
        results[size] = {}
        for algName, configs in algs.items():
            results[size][algName] = {}
            for configName, swapTypes in configs.items():
                results[size][algName][configName] = {}
                for swapTypeName, numSwaps in swapTypes.items():
                    results[size][algName][configName][swapTypeName] = {}
                    for numSwapsName, seeds in numSwaps.items():
                        sum = np.zeros(seeds.values()[0].shape)
                        n = 0
                        for seedName, m in seeds.items():
                            minRows = 0
                            minRows = min(sum.shape[0], m.shape[0])
                            sum = np.add(sum[:minRows,:], m[:minRows,:])
                            n = n+1
                        np.divide(sum, n)
                        results[size][algName][configName][swapTypeName][numSwapsName] = sum; 

    return results

#params == [n, alg, startConfig, swapType, numSwaps], use * for selecting any
def getPlots(values, result, params, prefix):
    if len(params) == 0:
        values.append( (prefix, result) )
    elif params[0] == '*':
        for name, rest in result.items():
            getPlots(values, rest, params[1:], prefix+name+"-")
    elif '-' in params[0]:
        for name in string.split(params[0], '-'):
            print name
            getPlots(values, result[name], params[1:], prefix+name+"-")
    else:
        getPlots(values, result[params[0]], params[1:], prefix)
    return values

def plotInversions(plots,x=3,y=0):
    rows = 10000
    for name, vals in plots:
        #This plots better, maybe we should put a hard stop at 10000 iterations?
        #line, = plt.plot(vals[:rows,3], vals[:rows,0])
        line, = plt.plot(vals[:,x], vals[:,y])
        line.set_label(name[:-1])
    plt.legend()
    ax=plt.gca()
    colormap = plt.get_cmap('prism')
    ax.set_color_cycle([colormap(k) for k in np.linspace(0, 1, 20)])
    plt.show()



