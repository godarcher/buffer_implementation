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
  if (userbound == 0)
  {
    writeToLog("operation failed: invalid value 0 for parameter userbound");
    cout << "you are putting in an invalid value (0), bound has to be > 0" << endl;
  }
  else if (userbound < 0)
  {
    writeToLog("operation failed: negative value for parameter userbound");
    cout << "you are putting in an invalid value (negative), bound has to be > 0" << endl;
  }
  else
  {
    writeToLog("operation succeeded: set: " + to_string(userbound) + " as buffer bound");
    bounded = true;
    bufferbound = userbound;
  }
}

void removeBufferBound()
{
  bounded = false;
  // we do not have to reset the bound (because it will be reassigned when enabled again)
}

void printBuffer()
{
  for (int element : buffer)
    std::cout << element << ' ';
  std::cout << endl;
}

void printLogger()
{
  for (int element : logger)
    std::cout << element << ' ';
  std::cout << endl;
}

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
