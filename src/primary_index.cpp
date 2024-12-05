#ifndef PRIMARYINDEX
#define PRIMARYINDEX

// #include "bits/stdc++.h"
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <algorithm>
#include <fstream>


using namespace std;

struct PrimaryIndexRow
{
  char Id[15];
  long long byteOffset;
  PrimaryIndexRow() {};
  PrimaryIndexRow(const char* id, int byteOffset) : byteOffset(byteOffset)
  {
    strcpy(Id, id);
  };

  bool operator<(const PrimaryIndexRow &opj)
  {
    return strcmp(Id, opj.Id) == -1;
  }

  bool operator>(const PrimaryIndexRow &opj)
  {
    return strcmp(Id, opj.Id) == 1;
  }

  bool operator==(const PrimaryIndexRow &opj)
  {
    return strcmp(Id, opj.Id) == 0;
  }
};

class PrimaryIndex
{
private:
  string filePath;

  void writeOnFile(vector<PrimaryIndexRow> v)
  {
    fstream file;
    file.open(filePath, ios::out);
    for (PrimaryIndexRow item : v)
    {
      file.write((char *)&item, sizeof(item));
    }
    file.close();
  }

public:
  PrimaryIndex() {};
  PrimaryIndex(string filePath)
  {
    this->filePath = filePath;
  }

  vector<PrimaryIndexRow> getVector()
  {
    vector<PrimaryIndexRow> v;
    fstream file;
    file.open(filePath, ios::in);
    PrimaryIndexRow s;
    while (file.read((char *)&s, sizeof(s)))
    {
      v.push_back(s);
    }
    file.close();
    return v;
  }

  void update_index(const char *id, long long byteOffset)
  {
    long long offset = getByteOffset(id); // get the current offset of the id in the file
    if (offset != -1)                     // if the id is already in the index
    {
      cout << "id already exists in the index" << endl;
      return;
    }

    fstream file;
    file.open(filePath, ios::out | ios::app);
    file.seekp(0, ios::end);
    PrimaryIndexRow studentIndex;
    strcpy(studentIndex.Id, id);
    studentIndex.byteOffset = byteOffset;
    file.write((char *)&studentIndex, sizeof(studentIndex));
    // cout << "primary index " << studentIndex.Id << " " << studentIndex.byteOffset << endl; // print the id and offset
    file.close();
    sortIndex();
  }

  void sortIndex()
  {
    vector<PrimaryIndexRow> v = getVector();
    sort(v.begin(), v.end());
    writeOnFile(v);
  }

  long long getByteOffset(const char *id)
  {
    fstream file;
    file.open(filePath);
    file.seekg(0, ios::end);
    int fileSize = file.tellg();
    file.seekg(0, ios::beg);

    int left = 0, right = fileSize, mid;
    PrimaryIndexRow s;
    if (fileSize == 0)
      return -1;

    while (left <= right)
    {
      mid = left + (right - left) / 2;
      mid -= mid % sizeof(PrimaryIndexRow);
      // cout << "left " << left << " mid " << mid << " right " << right << endl;
      file.seekg(mid, ios::beg);
      file.read((char *)&s, sizeof(s));
      // cout << s.id << " " << s.RRN << endl;
      if (strcmp(s.Id, id) == 0)
      {
        return s.byteOffset;
      }
      else if (strcmp(s.Id, id) == -1)
      {
        left = file.tellg();
        if (left >= fileSize)
          return -1;
      }
      else
      {
        right = mid - sizeof(PrimaryIndexRow);
      }
    }
    return -1;
  }

  bool deleteID(const char* id)
  {
    long long byteOffset = getByteOffset(id);
    if (byteOffset != -1)
    {
      const PrimaryIndexRow row(id, byteOffset);
      vector<PrimaryIndexRow> v = getVector();
      // auto pos = binary_search(v.begin(), v.end(), row);

      int it = find(v.begin(), v.end(), row) - v.begin();

      // auto it = find_if(v.begin(), v.end(), [id](StudentPrimaryIndexRow a){return a.id == id;});
      // int pos = it - v.begin();

      // cout << "id: " << id << " RRN: " << byteOffset << " pos: " << pos << endl;
      v.erase(v.begin() + it);
      writeOnFile(v);
      return true;
    }
    return false;
  }
};
#endif