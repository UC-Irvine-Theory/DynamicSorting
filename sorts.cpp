#include <functional>
#include <iostream>
#include <math.h>
#include <random>
#include <unordered_map>

#include "sorts.h"

void 
repeatedInsertionSort(EvolvingList& list, long time_limit)
{
  int n = list.size();
  while(list.getTime() < time_limit)
  {
    for(int i = 1; i < n; i++)
    {
      int j = i;
      while(j > 0 and list.compare(j,j-1))
      {
        list.swap(j-1,j);
        j--;
      }
    }
  }
}

void repeatedBubbleSort(EvolvingList& list, long time_limit)
{
  int n = list.size();
  while(list.getTime() < time_limit)
  {
    for(int i = 0; i < n-1; i++)
      if(list.compare(i+1,i))
        list.swap(i,i+1);
  }
}

void repeatedCocktailSort(EvolvingList& list, long time_limit)
{
  int n = list.size();
  while(list.getTime() < time_limit)
  {
    for(int i = 0; i < n-1; i++)
      if(list.compare(i+1,i))
        list.swap(i,i+1);
    for(int i = n-2; i >= 0; i--)
      if(list.compare(i+1,i))
        list.swap(i,i+1);
  }
}

void randomAdjacentCompareAndSwaps(EvolvingList& list, long time_limit)
{
  int n = list.size();
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0,n-2);
  auto get_index = std::bind(distribution,generator);

  while(list.getTime() < time_limit)
  {
    for(int i = 0; i < 1000; i++)
    {
      int index = get_index();
      if(list.compare(index+1,index))
        list.swap(index,index+1);
    }
  }
}

int partition(EvolvingList& list, std::vector<int>& toSort, int low, int high)
{
  int range = high - low + 1;
  int pivotChoice = low + std::rand() % range;
  std::swap(toSort[high], toSort[pivotChoice]);

  int i = low - 1;
  for(int j = low; j < high; j++)
  {
    if(list.compare(toSort[j],toSort[high]))
    {
      i++;
      std::swap(toSort[i],toSort[j]);
    }
  }
  std::swap(toSort[i+1],toSort[high]);
  return i+1;
}

void quicksort(EvolvingList& list, std::vector<int>& toSort, int low, int high)
{
  if(low < high)
  {
    int pivot = partition(list, toSort, low, high);
    quicksort(list,toSort,low,pivot-1);
    quicksort(list,toSort,pivot+1,high);
  }
}

void quicksort(EvolvingList& list, int n)
{
  std::vector<int> toSort(n);
  for(int i = 0; i < n; i++)
    toSort[i] = i;
  quicksort(list,toSort,0,n-1);
  list.permuteAnswer(toSort);
}

void quickThenInsertionSort(EvolvingList& list, long timeLimit)
{
  int n = list.size();
  quicksort(list,n);
  repeatedInsertionSort(list,timeLimit);
}

void repeatedQuicksort(EvolvingList& list, long timeLimit)
{
  int n = list.size();
  while(list.getTime() < timeLimit)
    quicksort(list,n);
}

void startNewQuicksortCall(std::vector<int>& toSort, std::stack<QSortState>& callStack, int low, int high)
{
  if(low < high)
  { 
    int range = high - low + 1;
    int pivotChoice = low + rand()%range;
    std::swap(toSort[high],toSort[pivotChoice]);
    QSortState newCall(low,high);
    callStack.push(newCall);
  }
}

bool stackQuicksortRunStep(EvolvingList& list, std::vector<int>&toSort, std::stack<QSortState>& callStack)
{
  bool stepPerformed = false; 
  
  while(not stepPerformed)
  {
    //If there are no quicksort calls on the stack, terminate
    if(callStack.empty())
      return false;
 
    //Otherwise attempt to run a step of the top call
    QSortState& currCall = callStack.top();
    int low = currCall.low;  
    int high = currCall.high;  
    int i = currCall.i;
    int j = currCall.j;  
  
    if(j < high)
    {
      if(list.compare(toSort[j],toSort[high]))
      {
        currCall.i++;
        std::swap(toSort[i+1],toSort[j]);
      }
      currCall.j++;
      stepPerformed = true;
    }
    else
    {
      callStack.pop();
      //If the quicksort call finished
      //  -move the pivot into the correct position
      std::swap(toSort[i+1],toSort[high]);

      //  -and start the two new recursive calls
      int lowLeft = low;
      int highLeft = i;
      int lowRight = i+2;
      int highRight = high;     
      startNewQuicksortCall(toSort,callStack,lowLeft,highLeft);
      startNewQuicksortCall(toSort,callStack,lowRight,highRight);
    }
  }
  return true;
}

void stackQuicksort(EvolvingList& list, int timeLimit)
{
  int n = list.size();
  while(list.getTime() < timeLimit)
  {
    std::stack<QSortState> callStack;
    std::vector<int> toSort(n);
    for(int i = 0; i < n; i++)
      toSort[i] = i;
    startNewQuicksortCall(toSort,callStack,0,n-1);
    while(stackQuicksortRunStep(list,toSort,callStack)); 
    list.permuteAnswer(toSort);
  }
}

void permuteVector(std::vector<int>& vec, std::vector<int>& perm)
{
  int n = perm.size();
  std::vector<int> oldVec(vec);
  for(int i = 0; i < n; i++)
  {
    vec[i] = perm[oldVec[i]];
  }
}

void blockedQuicksort(EvolvingList& list, int timeLimit)
{
  int n = list.size();
  
  //Find a block size close to 10 ln n
  int m = 10*log(n);
  if(m % 2 == 1)
    m++;
  if(m > n)
    m = n;
  while(n % m != 0) //Hopefully n isn't prime
    m+=2;

  std::stack<QSortState> fullStack;
  std::vector<int> toFullSort(n);
  for(int i = 0; i < n; i++)
    toFullSort[i] = i;
  //Begin with a normal quicksort run
  startNewQuicksortCall(toFullSort,fullStack,0,n-1);
  while(stackQuicksortRunStep(list,toFullSort,fullStack)); 
  list.permuteAnswer(toFullSort);
  
  std::vector<int> lastFullAnswer(n);
  std::vector<int> nextFullAnswer(n);
  for(int i = 0; i < n; i++)
  {
    lastFullAnswer[i] = i;
    nextFullAnswer[i] = i;
  }
  bool newFullAnswer = false;

  startNewQuicksortCall(toFullSort,fullStack,0,n-1);
  std::stack<QSortState> blockStack;

  while(list.getTime() < timeLimit)
  {
    std::vector<int> toBlockSort(m);
    std::vector<int> blockAnswer(n);

    //Switch to the answer from the newest full quicksort run
    if(newFullAnswer)
    {
      newFullAnswer = false;
      lastFullAnswer = nextFullAnswer;
    }

    //Start with the m/2 smallest elements in the block
    for(int i = 0; i < m/2; i++)
      toBlockSort[i] = lastFullAnswer[i];
    for(int i = 1; i < 2*n/m; i++)
    {
      //Place the next m/2 smallest elements in the block
      for(int j = 0; j < m/2; j++)
      {
        toBlockSort[m/2 + j] = lastFullAnswer[i*m/2 + j];
      }

      startNewQuicksortCall(toBlockSort,blockStack,0,m-1);
      while(stackQuicksortRunStep(list,toBlockSort,blockStack))
      {
        bool fullQuickSortFinished = not stackQuicksortRunStep(list,toFullSort,fullStack);
        if(fullQuickSortFinished)
        {
          newFullAnswer = true;
          nextFullAnswer = toFullSort;
          startNewQuicksortCall(toFullSort,fullStack,0,n-1);
        }
      }

      //Copy over the smallest m/2 elements in the block to the answer
      // and shift over the largest m/2 elements to the left
      for(int j = 0; j < m/2; j++)
      {
        blockAnswer[(i-1)*m/2 + j] = toBlockSort[j];
        toBlockSort[j] = toBlockSort[m/2+j];
      }
    }
    //Copy over the last set of m/2 elements in the block into the answer
    for(int j = 0; j < m/2; j++)
    {
      blockAnswer[n - m/2 + j] = toBlockSort[j];
    }
    list.permuteAnswer(blockAnswer);
    //After publishing new answer, update several lists of element indexes
    permuteVector(lastFullAnswer,blockAnswer); 
    permuteVector(nextFullAnswer,blockAnswer); 
    permuteVector(toFullSort,blockAnswer); 
  }
}
