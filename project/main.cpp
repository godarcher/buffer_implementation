/**
  * Assignment: synchronization
  * Operating Systems
  */

/**
  Hint: F2 (or Control-klik) on a functionname to jump to the definition
  Hint: Ctrl-space to auto complete a functionname/variable.
  */

// function/class definitions you are going to use
#include <algorithm>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

// although it is good habit, you don't have to type 'std::' before many objects by including this line
using namespace std;

// for now we declare the important stuff here
//TODO decide where this stays here
vector<int> buffer;
vector<string> logger;

//TODO We do all synchronization of these functions later on (happens in general function not in these ones)
void writeToLog(string log)
{
  logger.push_back(log);
}

string readFromLog(int index)
{
  return logger[index];
}

//TODO We do all synchronization of these functions later on (happens in general function not in these ones)
void writeToBuffer(int element)
{
  buffer.push_back(element);
}

void removeFromBuffer(int index)
{
  //? This is a little more complicated
  //* 1 copy buffer
  //* 2 remove all elements till reached index is removed
  //* 3 add back all elements after index (that are stored in different vector shortly)
  //* 4 save result to buffer.
  //! all of this can be done by the erase function of c++
  buffer.erase(index);
}

int main(int argc, char *argv[])
{
  return 0;
}
