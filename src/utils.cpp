#include "utils.h"
#include <algorithm>

using namespace std;

// Convert a string to uppercase
string toUpper(const string &str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// Split a string by a delimiter
vector<string> split(const string &str, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream ss(str);
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Perform a binary search in a sorted vector
int binarySearch(const vector<string> &data, const string &key) {
    int low = 0, high = data.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (data[mid] == key) return mid;
        if (data[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1; // Not found
}
