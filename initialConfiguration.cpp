#include "initialConfiguration.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

    std::unordered_map<std::string, InitialConfiguration::CONFIG_TYPE>InitialConfiguration::configMap = {
        {"SHUFFLE"           , CONFIG_TYPE::SHUFFLE},
        {"LOG_SHUFFLE"       , CONFIG_TYPE::LOG_SHUFFLE},
        {"LOGLOG_SHUFFLE"    , CONFIG_TYPE::LOGLOG_SHUFFLE},
        {"SORTED"            , CONFIG_TYPE::SORTED},
        {"REVERSE"           , CONFIG_TYPE::REVERSE},
        {"PEAK"              , CONFIG_TYPE::PEAK},
        {"LOG_STAIR"         , CONFIG_TYPE::LOG_STAIR},
        {"LOGLOG_STAIR"      , CONFIG_TYPE::LOGLOG_STAIR},
        {"REVERSE_LOG_STAIR" , CONFIG_TYPE::REVERSE_LOG_STAIR},
    };

InitialConfiguration::CONFIG_TYPE
InitialConfiguration::getType(std::string const& name) const
{
    if( configMap.count(name) == 0)
    {
        return CONFIG_TYPE::INVALID;
    }
    return configMap.at(name);
}

std::vector<unsigned int> 
InitialConfiguration::generateConfiguration(unsigned int n, InitialConfiguration::CONFIG_TYPE type)
{
    if(type == CONFIG_TYPE::INVALID)
        return std::vector<unsigned int>();

    std::vector<unsigned int> startConfig;
    for(unsigned int i = 0; i < n; ++i) startConfig.push_back(i);

    switch(type)
    {
        case CONFIG_TYPE::SHUFFLE:
            std::shuffle(startConfig.begin(), startConfig.end(), rng);
            break;
        case CONFIG_TYPE::LOG_SHUFFLE:
        {
            unsigned int step = std::log2(n);
            stepShuffle(startConfig.begin()       , startConfig.end(), step);
            stepShuffle(startConfig.begin()+step/2, startConfig.end(), step);
            break;
        }
        case CONFIG_TYPE::LOGLOG_SHUFFLE:
        {
            unsigned int step = std::log2(std::log2(n));
            stepShuffle(startConfig.begin(), startConfig.end(), step);
            if(step > 3)
                stepShuffle(startConfig.begin()+step/2, startConfig.end(), step);
            break;
        }

        case CONFIG_TYPE::SORTED:
            break;
        case CONFIG_TYPE::REVERSE:
            std::reverse(startConfig.begin(), startConfig.end());
            break;
        case CONFIG_TYPE::PEAK:
        {
            auto midpoint = startConfig.begin() + n/2;
            std::rotate(startConfig.begin(), midpoint, startConfig.end());
            break;
        }
        
        case LOG_STAIR:
        {
            unsigned int step = std::log2(n);
            stepReverse(startConfig.begin(), startConfig.end(), step);
            break;
        }
        case LOGLOG_STAIR:
        {
            unsigned int step = std::log2(std::log2(n));
            stepReverse(startConfig.begin(), startConfig.end(), step);
            break;
        }
        case REVERSE_LOG_STAIR:
        {
            unsigned int step = std::log2(n);
            stepReverse(startConfig.begin(), startConfig.end(), step);
            std::reverse(startConfig.begin(), startConfig.end());
            break;
        }
        default: // DO nothing
            break;
    }

    return startConfig;
}
std::vector<unsigned int>
InitialConfiguration::generateConfiguration(unsigned int n, std::string const& name)
{
    return generateConfiguration(n, getType(name));
}

void
InitialConfiguration::stepShuffle(std::vector<unsigned int>::iterator begin, std::vector<unsigned int>::iterator end, int step)
{
    int times = (end - begin) / step;
    auto tail = begin;
    auto head = begin + step;

    for(int i = 0; i < times; ++i)
    {
        std::shuffle(tail,head, rng);
        tail = head;
        head = head + step;
    }
    std::shuffle(tail, end, rng); // Last piece
}

void
InitialConfiguration::stepReverse(std::vector<unsigned int>::iterator begin, std::vector<unsigned int>::iterator end, int step)
{
    int times = (end - begin) / step;
    auto tail = begin;
    auto head = begin + step;

    for(int i = 0; i < times; ++i)
    {
        std::reverse(tail,head);
        tail = head;
        head = head + step;
    }
    std::reverse(tail, end); // Last piece
}

void
InitialConfiguration::printConfigurations(int n)
{
    auto shuffle       = generateConfiguration(n, CONFIG_TYPE::SHUFFLE);
    auto logShuffle    = generateConfiguration(n, CONFIG_TYPE::LOG_SHUFFLE);
    auto loglogShuffle = generateConfiguration(n, CONFIG_TYPE::LOGLOG_SHUFFLE);
    
    auto sorted  = generateConfiguration(n, CONFIG_TYPE::SORTED);
    auto reverse = generateConfiguration(n, CONFIG_TYPE::REVERSE);
    auto peak    = generateConfiguration(n, CONFIG_TYPE::PEAK);

    auto logStair     = generateConfiguration(n, CONFIG_TYPE::LOG_STAIR);
    auto loglogStair  = generateConfiguration(n, CONFIG_TYPE::LOGLOG_STAIR);
    auto rlogStair    = generateConfiguration(n, CONFIG_TYPE::REVERSE_LOG_STAIR);

    std::cout << "SHUFFLE,LOG_SHUFFLE,LOGLOG_SHUFFLE,SORTED,REVERSE,PEAK,LOG_STAIR,LOGLOG_STAIR,REVERSE_LOG_STAIR " << std::endl;
    for(int i = 0; i < n; ++i)
    {
        std::cout << std::left << std::setw(7)  << shuffle[i]       << ",";
        std::cout << std::left << std::setw(11) << logShuffle[i]    << ",";
        std::cout << std::left << std::setw(14) << loglogShuffle[i] << ",";

        std::cout << std::left << std::setw(6)  << sorted[i]  << ",";
        std::cout << std::left << std::setw(7)  << reverse[i] << ",";
        std::cout << std::left << std::setw(4)  << peak[i]    << ",";
        
        std::cout << std::left << std::setw(9)  << logStair[i]     << ",";
        std::cout << std::left << std::setw(13) << loglogStair[i]  << ",";
        std::cout << std::left << std::setw(19) << rlogStair[i]    << "";

        std::cout << std::endl;
    }
}


