#include "evolvingList.h"

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <random>
#include <vector>

EvolvingList::EvolvingList(std::vector<unsigned int> const& startConfig, unsigned int sampleRate_, SwapType swapType_, unsigned int swapRate_, unsigned int seed)
: n(startConfig.size()), sampleRate(sampleRate_), swapType(swapType_), swapRate(swapRate_)
{
    std::srand(seed);
    rng = std::mt19937_64(seed);
    swapDist = std::uniform_int_distribution<int>(0,n-2);

    real.assign(n , Element());
    aprox.assign(n, Element());

    for(int i = 0; i < n; ++i)
    {
        real[i].ptr               = &aprox[startConfig[i]];
        aprox[startConfig[i]].ptr = &real[i];
    }

    stats.setInversionCount(totalInversions());
    stats.sampleStat();
}

int 
EvolvingList::compare(unsigned int i, unsigned int j)
{
   if(stats.getCompares() % sampleRate == 0) // Probably better to use a dedicated counter
   {
       stats.setInversionCount(totalInversions());
       stats.sampleStat();
   }
   stats.addCompare();
   
   for(int i = 0; i < swapRate; i++)
   {
       if(swapType == SwapType::RANDOMSWAP)
           randomSwap();
       else if(swapType == SwapType::HOTSPOT)
           hotSpotMovement();
   }

   return aprox[i].ptr < aprox[j].ptr;
}

void
EvolvingList::swap(unsigned int i, unsigned int j)
{
    //std::cout << "\t\tSwapping " << i << " and " << j << std::endl;
    std::swap( aprox[i],  aprox[j]);
    aprox[i].ptr->ptr = &aprox[i];
    aprox[j].ptr->ptr = &aprox[j];
}

/*
 * Applies a permutation to the answer list.
 */
void
EvolvingList::permuteAnswer(std::vector<int>& ansPerm)
{
    int n = real.size();
    std::vector<Element> oldApprox(aprox);
    for(int i = 0; i < n; i++)
    {
        aprox[i] = oldApprox[ansPerm[i]];
        aprox[i].ptr->ptr = &aprox[i];
    }
}

void
EvolvingList::printValues() const
{
    std::cout << "Inversions: " << totalInversions() << std::endl;
    std::cout << "i Real/Aprox\n";
    for(int i = 0; i < n; ++i)
    {
        std::cout << i  << " " <<
        (real[i].ptr  - &aprox[0])   << "/" << 
        (aprox[i].ptr - &real[0] )   << std::endl;
    }
}

unsigned long
EvolvingList::getInversions() const
{
    return totalInversions();
}

void
EvolvingList::sampleStat()
{
    stats.setInversionCount(getInversions());
    stats.sampleStat();
}

void
EvolvingList::randomSwap()
{
    unsigned int position = swapDist(rng);

    //Check if a good or bad swap
    if(real[position].ptr < real[position+1].ptr)
        stats.addBadSwap();
    else
        stats.addGoodSwap();

    std::swap(real[position], real[position+1]);
    real[position].ptr->ptr    = &real[position];
    real[position+1].ptr->ptr  = &real[position+1];
}

void 
EvolvingList::hotSpotMovement()
{
    //Select a uniformly random element to be the hot spot
    int hotIndex = std::rand() % n;
    //Decide if the hot spot is moving up or down
    bool up = std::rand() % 2 == 1;
    if(up)
    {
        //While a coin comes up heads move the hot spot
        while(hotIndex > 0 and std::rand()%2 == 1)
        {
            std::swap(real[hotIndex], real[hotIndex - 1]);
            real[hotIndex].ptr->ptr = &real[hotIndex];
            real[hotIndex - 1].ptr->ptr = &real[hotIndex - 1];
            hotIndex--;
        }
    }
    else
    {
        while(hotIndex < n-1 and std::rand()%2 == 1)
        {
            std::swap(real[hotIndex], real[hotIndex + 1]);
            real[hotIndex].ptr->ptr = &real[hotIndex];
            real[hotIndex + 1].ptr->ptr = &real[hotIndex + 1];
            hotIndex++;
        }
    }
}

unsigned long
EvolvingList::totalInversions() const
{
    std::vector<int> vec(n);
    for(int i = 0; i < n; i++)
        vec[i] = i;
    return mergeSortInversions(vec);
}

unsigned long 
EvolvingList::mergeSortInversions(std::vector<int>& v) const
{
    int n = v.size();
    if (n <= 1)
      return 0;
    std::vector<int> left(v.begin(),v.begin()+n/2);
    std::vector<int> right(v.begin()+n/2,v.end());
    unsigned long leftInvs = mergeSortInversions(left);
    unsigned long rightInvs = mergeSortInversions(right);
    unsigned long invsBetween = 0;
    unsigned int i = 0, j = 0, k = 0;
    while(i < left.size() and j < right.size())
    {
        if(aprox[left[i]].ptr < aprox[right[j]].ptr)
        {
            v[k] = left[i];
            k++,i++;
        } else {
            v[k] = right[j];
            k++,j++;
            invsBetween += (left.size() - i);
        }
    }
    while(i < left.size())
    {
        v[k] = left[i];
        k++,i++;
    }
    while(j < right.size())
    {
        v[k] = right[j];
        k++,j++;
    }
    return leftInvs + rightInvs + invsBetween;
}

