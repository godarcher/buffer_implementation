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
#include <string> //added by us

// although it is good habit, you don't have to type 'std::' before many objects by including this line
using namespace std;

// for now we declare the important stuff here
//TODO decide where this stays here
vector<int> buffer;
vector<string> logger;
bool bounded = false;
int bufferbound = 0;

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
  if (bounded == true)
  {
    if (buffer.size() < bufferbound)
    {
      // * There is room left so we push
      buffer.push_back(element);
      writeToLog("operation succeeded, added: " + to_string(element) + " to buffer");
    }
    else if (buffer.size() == bufferbound)
    {
      writeToLog("operation failed: The buffer has already reached its bound");
    }
  }
  else //not bounded
  {
    buffer.push_back(element);
    writeToLog("operation succeeded, added: " + to_string(element) + " to buffer");
  }
}

void setBufferBound(int userbound)
{
  bounded = true;
  bufferbound = userbound;
}

void removeBufferBound()
{
  bounded = false;
}

void printBuffer()
{
  for (int element : buffer)
    std::cout << element << ' ';
  std::cout << endl;
}

//? This is a little more complicated
//* 1 copy buffer
//* 2 remove all elements till reached index is removed
//* 3 add back all elements after index (that are stored in different vector shortly)
//* 4 save result to buffer.
//? all of this can be done by the erase function of c++
void removeFromBuffer(int index)
{
  buffer.erase(buffer.begin() + index);
}

void buffertests()
{
  std::cout << "empty buffer: " << endl;
  printBuffer();
  writeToBuffer(3);
  writeToBuffer(4);
  writeToBuffer(5);
  std::cout << "filled buffer: " << endl;
  printBuffer();
  removeFromBuffer(0);
  std::cout << "buffer - elem 0: " << endl;
  printBuffer();
}

int main(int argc, char *argv[])
{
  buffertests();
  return 0;
}
