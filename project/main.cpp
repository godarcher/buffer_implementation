/**
  * Assignment: synchronization
  * Operating Systems
  */

/**
  Hint: F2 (or Control-klik) on a functionname to jump to the definition
  Hint: Ctrl-space to auto complete a functionname/variable.
  */

//dependencies
#include <algorithm>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
//includes from here one are our own
#include <string>

using namespace std;

mutex bufmutex;
mutex logmutex;
mutex boundmutex;

// ! Global variables
vector<int> buffer;
vector<string> logger;
bool bounded = false;
int bufferbound = 0;

// * This function writes a single log to the log vector
void writeToLog(string log)
{
  //! entire function is critical
  logmutex.lock();
  logger.push_back(log);
  logmutex.unlock();
}

// * This function reads a specific log sample, by using an index as input
// * the function checks for wrong indexes and returns errors if there are any
string readFromLog(int index)
{
  int size = logger.size();

  //? if size = 1, we index 0
  //! BEGIN OF CRITICAL SECTION
  logmutex.lock();
  if (size >= index + 1)
  {
    string output = logger[index];
    logmutex.unlock();
    //? We use the stirng output because otherwise the return messes up our lock
    return output;
  }
  //! END OF CRITICAL SECTION
  else if (size < index + 1)
  {
    std::cout << "ERROR: index out of bounds" << endl;
    return "ERROR: index out of bounds";
  }
  else
  {
    //? not in size and not higher then size so negative
    std::cout << "ERROR: negative index" << endl;
    return "ERROR: negative index";
  }
  logmutex.unlock();
}

// * This function prints the entire log onto the terminal
// * It checks if the log is not empty, and shows that it is empty if it is.
void printLog()
{
  logmutex.lock();
  //! BEGIN OF CRITICAL SECTION
  if (logger.size() > 0)
  {
    for (string element : logger)
      std::cout << element << ' ';
    //! END OF CRITICAL SECTION
    logmutex.unlock();
    std::cout << endl;
  }
  else
  {
    logmutex.unlock();
    std::cout << "The log is empty " << endl;
  }
}

// * This function writes to the buffer, it therefore
// * Checks wether or not it is already full if bounded
// * the function also updates the log with operation about its success
void writeToBuffer(int element)
{
  bufmutex.lock();
  boundmutex.lock();
  //! ENTIRE FUNCTION IS CRITICAL
  if (bounded == true)
  {
    if (buffer.size() < bufferbound)
    {
      // ? There is room left so we push
      buffer.push_back(element);
      writeToLog("operation succeeded, added: " + to_string(element) + " to buffer");
    }
    else if (buffer.size() == bufferbound)
    {
      writeToLog("operation failed: The buffer has already reached its bound");
    }
    boundmutex.unlock();
  }
  else // ? not bounded
  {
    buffer.push_back(element);
    writeToLog("operation succeeded, added: " + to_string(element) + " to buffer");
  }
  bufmutex.unlock();
}

// * This function sets the max bound of the buffer
// * as base case we check for legitimate inputs
// * It also provides the log with information about its success
void setBufferBound(int userbound)
{
  boundmutex.lock();
  //! ENTIRE FUNCTION IS CRITICAL
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

    bufmutex.lock();
    // ? Case where buffer > new bound, we remove elements exceeding bound
    if (buffer.size() > bufferbound)
    {
      //? we basicly remove range(userbound --> end of buffer)
      buffer.erase(buffer.begin() + userbound, buffer.end());
    }
    bufmutex.unlock();
  }
  boundmutex.unlock();
}

// * This function removes the buffer bound
void removeBufferBound()
{
  //! ENTIRE FUNCTION IS CRITICAL
  bufmutex.lock();
  bounded = false;
  bufmutex.unlock();
  // we do not have to reset the bound (because it will be reassigned when enabled again)
}

// * This function prints out the entire buffer to the screen
// * It checks if the buffer is empty and shows the buffer is empty if it is.
void printBuffer()
{
  bufmutex.lock();
  //! BEGIN OF CRITICAL SECTION
  if (buffer.size() > 0)
  {
    for (int element : buffer)
      std::cout << element << ' ';
    bufmutex.unlock();
    //! END OF CRITICAL SECTION
    std::cout << endl;
  }
  else
  {
    bufmutex.unlock();
    std::cout << "buffer is empty" << endl;
  }
}

// * This function removes a specific element from the buffer
// * it also checks for wrong indexes
// * the function also writes to log about its succes
void removeFromBuffer(int index)
{
  bufmutex.lock();
  //! BEGIN OF CRITICAL SECTION
  int size = buffer.size();
  //size 1 = index 0, so index 1 should be illegal
  if (index < size)
  {
    buffer.erase(buffer.begin() + index);
    bufmutex.unlock();
    writeToLog("Operation succeeded: removed " + to_string(index) + " from buffer");
  }
  //! END OF CRITICAL SECTION
  else if (index >= size)
  {
    std::cout << "ERROR: index out of bounds" << endl;
    writeToLog("operation failed: out of bounds index supplied to remove");
  }
  else if (index < 0)
  {
    std::cout << "ERROR: negative index" << endl;
    writeToLog("operation failed: negative index supplied to remove");
  }
  bufmutex.unlock();
}

// * This function is used for testing some buffer operations
void buffertest1()
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

// * This function is used for testing some special cases
void buffertest2()
{
  printLog();           //empty log
  printBuffer();        //empty buffer
  readFromLog(10);      //out of bounds
  readFromLog(-3);      //negative index
  setBufferBound(0);    //empty buffer size
  setBufferBound(-3);   //negative buffer size
  removeFromBuffer(20); //out of bounds
  removeFromBuffer(-2); //negative index
}

// * This is the main function, it is called when the program is ran
int main(int argc, char *argv[])
{
  buffertest1();
  buffertest2();
  return 0;
}