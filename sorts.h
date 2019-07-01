#ifndef SORTS_H
#define SORTS_H

#include <stack>
#include <vector>

#include "evolvingList.h"

void repeatedInsertionSort(EvolvingList& list, long time_limit);

void repeatedBubbleSort(EvolvingList& list, long time_limit);

void repeatedCocktailSort(EvolvingList& list, long time_limit);

void randomAdjacentCompareAndSwaps(EvolvingList& list, long time_limit);

int partition(EvolvingList& list, std::vector<int>& toSort, int low, int high);

struct QSortState
{
  int low,high,i,j;
  QSortState(int l, int h)
  {
    low = l;
    high = h;
    i = low-1;
    j = low;
  }
};

void quicksort(EvolvingList& list, std::vector<int>& toSort, int low, int high);

void quicksort(EvolvingList& list, int n);

void quickThenInsertionSort(EvolvingList& list, long timeLimit);

void repeatedQuicksort(EvolvingList& list, long timeLimit);

void startNewQuicksortCall(std::vector<int>& toSort, std::stack<QSortState>& callStack, int low, int high);

bool stackQuicksortRunStep(EvolvingList& list, std::vector<int>&toSort, std::stack<QSortState>& callStack);

void stackQuicksort(EvolvingList& list, int timeLimit);

void permuteVector(std::vector<int>& vec, std::vector<int>& perm);

void blockedQuicksort(EvolvingList& list, int timeLimit);

#endif //  SORTS_H
