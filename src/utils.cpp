#include "utils.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

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
}
int binarySearch(fstream &indexfile, const string &key, int recordSize, int keySize) {

    indexfile.seekg(0, ios::end);
    int fileSize = indexfile.tellg();
    indexfile.seekg(0, ios::beg);

    int low = 0, high = fileSize / recordSize - 1; // assuming each key is 4 bytes
    while (low <= high) {
        int mid = low + (high - low) / 2;
        indexfile.seekg(mid * recordSize, ios::beg);
        cout << indexfile.tellg() << endl;
        char possiblekey[keySize + 1] = {0}; // +1 for null terminator
        indexfile.read(possiblekey, keySize);
        possiblekey[keySize] = '\0'; // Ensure null termination

        cout << possiblekey << " " << low << " " << high << endl;
        string possibleKeyStr(possiblekey);
        if (possibleKeyStr.compare(key) == 0) return mid;
        if (possibleKeyStr.compare(key) < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1; // Not found
}
