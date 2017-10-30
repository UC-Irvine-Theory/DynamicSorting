#include <iostream>
#include <random>
#include <vector>
#include <unordered_map>


class InitialConfiguration
{
public:
    // TODO: Change Name of PEAK
    enum CONFIG_TYPE {
          SHUFFLE, LOG_SHUFFLE, LOGLOG_SHUFFLE, 
          SORTED, REVERSE, PEAK, 
          LOG_STAIR, LOGLOG_STAIR, REVERSE_LOG_STAIR,
          INVALID,
    };

    InitialConfiguration(int seed)
    : rng(seed)
    {}

    // Returns a randomized according to type permutation
    std::vector<unsigned int> generateConfiguration(unsigned int n, CONFIG_TYPE type);
    std::vector<unsigned int> generateConfiguration(unsigned int n, std::string const& name);

    void printConfigurations(int n);

    // Return INVALID if type doesn't exist
    CONFIG_TYPE getType(std::string const& name) const;

private:
    static std::unordered_map<std::string, CONFIG_TYPE> configMap;
public:
    static void printConfigNames()
    {
        for(auto name : configMap)
            std::cout << " " << name.first;
    }

private:
    void stepShuffle(std::vector<unsigned int>::iterator begin, std::vector<unsigned int>::iterator end, int step);

    void stepReverse(std::vector<unsigned int>::iterator begin, std::vector<unsigned int>::iterator end, int step);

    std::default_random_engine rng;
};
