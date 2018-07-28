#ifndef LARSTUDIESTOOLS
#define LARSTUDIESTOOLS

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#include <iostream>
#include <vector>
#include <string.h>
#include <TString.h>

using namespace std;
std::vector<string> split(const string &text, char sep);
std::string findMatchInDir(const char* dirpath, const char* match);
std::vector<std::string> findSamplesInDir(const char* dirpath, const char* match);

#endif
