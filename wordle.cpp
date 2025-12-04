#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
bool isWord(const std::string& in, const std::set<std::string>& set);
void generateWord(const std::string& in, std::string& current, int pos, const std::set<std::string>& dict,
                  std::set<std::string>& results, const std::string& floating);
bool floatingFilter(const std::string& in, const std::string& floating);


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    std::set<std::string> results;
    std::set<std::string> shortDict;
    std::string current = in;
    std::set<std::string>::iterator it;

    // Create shorter dictionary to cut back on lookup time
    for (it = dict.begin(); it != dict.end(); ++it) {
      if ((*it).size() == in.size()) {
        shortDict.insert(*it);
      }
    }

    generateWord(in, current, 0, shortDict, results, floating);
    return results;
}



// checks if word is valid (in the short dictionary)
bool isWord(const std::string& in, const std::set<std::string>& dict)
{
    std::set<std::string>::iterator it = dict.find(in);
    if (it != dict.end()) {
      return true;
    }
    return false;
}


// checks if partially constructed word could be valid, put those valid words
// into new set to check in further recursions
std::set<std::string> pruneWord(const std::string& in, const std::set<std::string>& dict, int pos)
{
    std::set<std::string> possibleNext;
    std::set<std::string>::iterator it;
    bool good = false;

    for (it = dict.begin(); it != dict.end(); ++it) {
      good = true;

      for (int i = 0; i <= pos; i++) {
        if (in[i] != (*it)[i]) {
          good = false;
          break;
        }
      }

      if (good) {
        possibleNext.insert(*it);
      }
    }

    return possibleNext;
}


// recursively generate words
void generateWord(const std::string& in, std::string& current, int pos, const std::set<std::string>& dict,
                  std::set<std::string>& results, const std::string& floating)
{
  int length = in.size();

  // Base case since max pos is 1 less than length
  if (pos == length) {
    if (floatingFilter(current, floating) && isWord(current, dict)) {
      results.insert(current);
    }
    return;
  }

  // Skip position if already filled out
  if (in[pos] != '-') {
    current[pos] = in[pos];
    generateWord(in, current, pos+1, dict, results, floating);
  }

  // Try every letter and possibly recurse 
  else {
    for (char c = 'a'; c <= 'z'; c++) {
      //std::string temp = in;
      current[pos] = c;
      //temp[pos] = c;

      // Create smaller dictionaries of possible next steps, to reduce going through
      // large dictionary many many times
      std::set<std::string> possibleNext = pruneWord(current, dict, pos);
      if (!possibleNext.empty()) {
        generateWord(in, current, pos+1, possibleNext, results, floating);
      }
    }
  }
}


// check if final word contains all the floating letters
bool floatingFilter(const std::string& in, const std::string& floating)
{
    std::string temp = in;

    for (size_t i = 0; i < floating.size(); i++) {
      char c = floating[i];
      size_t pos = temp.find(c);
      if (pos == string::npos) {
        return false;
      }
      // In case of duplicate letters in floating, remove 
      temp.erase(pos, 1);
    }
  return true;
}


