#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

#include "evolvingList.cpp"
#include "initialConfiguration.h"
#include "sorts.cpp"
using Stats = EvolvingList::Stats;

std::unordered_map<std::string, std::function<void(EvolvingList&,long)> > sortingAlgorithms =
{
    { "INSERTION", repeatedInsertionSort},
    { "QUICKINSERTION", quickThenInsertionSort},
    { "BUBBLE", repeatedBubbleSort},
    { "COCKTAIL", repeatedCocktailSort},
    { "SWAPS", randomAdjacentCompareAndSwaps},
    { "QUICK", repeatedQuicksort},
    { "STACKQUICK", stackQuicksort},
    { "BLOCKQUICK", blockedQuicksort},
};

std::unordered_map<std::string, EvolvingList::SwapType> swapTypes =
{
    {"RANDOM", EvolvingList::RANDOMSWAP},
    {"HOTSPOT", EvolvingList::HOTSPOT},
};

void printUsage()
{
    std::cout << "USAGE: <n> <alg> "
              << "<swap type> <swap rate>"
              << "<seed> <initialConfig>  "
              << "<sampleRate> <timiLimit> "
              << "[outputFile] [dist]" << std::endl;
    std::cout << "Where:" << std::endl;
    std::cout << "\tn: int > 0" << std::endl;

    std::cout << "\talg   : is one of {";
    for(auto name : sortingAlgorithms)
        std::cout << " " << name.first;
    std::cout << "}" << std::endl;

    std::cout << "\tswap type: is one of {";
    for(auto name : swapTypes)
        std::cout << " " << name.first;
    std::cout << "}" << std::endl;

    std::cout << "\tswap rate: int > 0" << std::endl;

    std::cout << "\tseed  : int > 0" << std::endl;
    

    std::cout << "\tinitialConfig   : is one of {";
    InitialConfiguration::printConfigNames();
    std::cout << "}\n";

    std::cout << "\tsampleRate: int  > 0" << std::endl;
    std::cout << "\ttimeLimit : long > 0" << std::endl;
}

template<typename SortingAlgorithm>
Stats runTest(std::vector<unsigned int> const& startConfig, 
              SortingAlgorithm& alg, 
              const int sampleRate, const long timeLimit, 
              const EvolvingList::SwapType swapType, const unsigned int swapRate,
              const unsigned seed)
{
    EvolvingList values(startConfig, sampleRate, swapType, swapRate, seed);
    std::cout << "Inversions: " << values.getInversions() << std::endl;

    alg(values,timeLimit);

    return values.getStats();
}

int main(int argc, char *argv[])
{
    if(argc < 9)
    {
        printUsage();
        return 0;
    }

    const int n = std::stoi(argv[1]);

    const std::string algorithmName = argv[2];
    if( sortingAlgorithms.count(algorithmName) == 0)
    {
        std::cout << "Algorithm doesn't exist!, you gave " << argv[2] << std::endl;
        printUsage();
        return 0;
    }

    const std::string swapTypeName = argv[3];
    if(swapTypes.count(swapTypeName) == 0)
    {
        std::cout << "Swap type doesn't exist!, you gave " << argv[2] << std::endl;
        printUsage();
        return 0;
    }

    const unsigned int swapRate = std::stoi(argv[4]);

    const int seed = std::stoi(argv[5]);

    const std::string initialConfigName = argv[6];
    InitialConfiguration initialConfiguration(seed);
    if(initialConfiguration.getType(initialConfigName) == InitialConfiguration::INVALID)
    {
        std::cout << "Initial configuration is invalid you gave" << argv[3] << std::endl;
        printUsage();
    }

    std::vector<unsigned int> startConfig = initialConfiguration.generateConfiguration(n, initialConfigName);

    const int  sampleRate  = std::stoi(argv[7]);
    const long timeLimit   = std::stol(argv[8]);

    std::ostream* out = &std::cout;
    std::ofstream myfile;
    if(argc > 9)
    {
        myfile.open (argv[9]);
        if(!myfile.is_open())
        {
            std::cout << "Couldn't open file " << argv[9] << std::endl;
            return 1;
        }
        out = &myfile;
    }

    Stats stats = runTest(startConfig, sortingAlgorithms[algorithmName], sampleRate, timeLimit, swapTypes[swapTypeName], swapRate, seed);

    *out << stats;

    if(myfile.is_open())
        myfile.close();

    return 0;
}
