#!/bin/bash
for n in 100 200; do
    for alg in INSERTION BUBBLE COCKTAIL QUICK BLOCKQUICK; do
    #for alg in INSERTION BUBBLE ; do
        for seed in 0 1 ; do
            for config in SHUFFLE SORTED; do
                for swapType in RANDOM HOTSPOT; do
                    for numSwaps in 1 2; do
                        sampleRate=$(( $n/20))
                        timeLimit=$(( $n*$n))

                        filename="results/$alg-$n-$config-$swapType-$numSwaps-$seed.out"
            
                        ./Sort $n $alg $swapType $numSwaps $seed $config $sampleRate $timeLimit ./$filename

                        i=$(($i+1))
                    done
                done
            done
        done
    done
done
