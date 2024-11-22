#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <sstream>

using namespace std;

string toUpper(const string &str);
vector<string> split(const string &str, char delimiter);
int binarySearch(const vector<string> &data, const string &key);

#endif
