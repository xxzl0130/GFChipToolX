#ifndef __SPLIT_H
#define __SPLIT_H

//from https://github.com/ekg/split/blob/master/split.cpp

// functions to split a string by a specific delimiter
#include <string>
#include <vector>
#include <sstream>
#include <string.h>
#include "chipStdafx.h"

// thanks to Evan Teran, http://stackoverflow.com/questions/236129/how-to-split-a-string/236803#236803

// split a string on a single delimiter character (delim)
DLL_EXP std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
DLL_EXP std::vector<std::string> split(const std::string &s, char delim);

#endif
