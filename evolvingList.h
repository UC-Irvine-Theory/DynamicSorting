#ifndef EVOLVING_LIST_H
#define EVOLVING_LIST_H


#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>

class EvolvingList
{


    struct Element
    {
        Element()
        : ptr(NULL)
        {}

        Element(Element* ptr_)
        :ptr(ptr_)
        {}

        Element* ptr;
    };

public:

    enum class SwapType
    {
        RANDOMSWAP, HOTSPOT
    };

    class Stats
    {
    private:
        struct SingleStat
        {
            long compares       = 0;
            long inversionCount = 0;
            long goodSwaps      = 0;
            long badSwaps       = 0;

            friend std::ostream& operator<<(std::ostream &output, const SingleStat& stat ) { 
                output << stat.inversionCount << "," 
                       << stat.goodSwaps      << "," 
                       << stat.badSwaps       << ",";
                output << stat.compares ;
                return output;            
            }
        };

    public:
        void addCompare()  { ++currentStat.compares;  }
        void addGoodSwap() { ++currentStat.goodSwaps; }
        void addBadSwap()  { ++currentStat.badSwaps;  }
        void setInversionCount(long newCount) { currentStat.inversionCount = newCount;}

        void sampleStat() { allStats.push_back(currentStat); }
        int getCompares() const { return currentStat.compares;}

        friend std::ostream& operator<<(std::ostream &output, const Stats& stats ) { 
            for(auto const& s : stats.allStats)
                output << s << std::endl;
            output <<  stats.currentStat << std::endl;
            return output;            
        }

        SingleStat currentStat;
        std::vector<SingleStat> allStats;
    };

    EvolvingList(std::vector<unsigned int> const& startConfig, unsigned int sampleRate_, SwapType swapType_, unsigned int swapRate_, unsigned int seed);

    int 
    compare(unsigned int i, unsigned int j);

    void swap(unsigned int i, unsigned int j);

    /*
     * Applies a permutation to the answer list.
     */
    void permuteAnswer(std::vector<int>& ansPerm);

    void printValues() const;

    unsigned int size() const { return n; }

    unsigned long getInversions() const;

    void sampleStat();

    Stats getStats() const { return stats; }

    int getTime() const { return stats.getCompares(); }

    unsigned long totalInversions() const;
    
    unsigned long mergeSortInversions(std::vector<int>& v) const;

private:

    void randomSwap();

    void hotSpotMovement();

    // The "true" value of an element is the index in the real list
    // The elements are linked by pointers which must be maintained when doind swaps
    std::vector<Element>  real; // The list seen by the random process
    std::vector<Element> aprox; // The list seen by the sorting algorithm
    const int n;

    Stats stats;
    const int sampleRate;


    const SwapType swapType;
    const int swapRate;

    // Random generator stuff, should be encapsulated in a SwapGenerator class
    std::mt19937_64 rng;
    std::uniform_int_distribution<int> swapDist;
};

#endif //  EVOLVING_LIST_H
