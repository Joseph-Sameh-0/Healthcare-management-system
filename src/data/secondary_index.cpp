#ifndef SECONDARYINDEX
#define SECONDARYINDEX

#include "bits/stdc++.h"
using namespace std;

template <class T>
struct SecondaryIndexRow
{
  T Key;
  char Id[15];
  SecondaryIndexRow() {};
  SecondaryIndexRow(T key, char id[])
  {
    strcpy(Id, id);
    strcpy(Key, key);
  };

  bool operator<(const SecondaryIndexRow &opj)
  {
    return (strcmp(Key, opj.Key) == -1);
  }

  bool operator>(const SecondaryIndexRow &opj)
  {
    return (strcmp(Key, opj.Key) == 1);
  }

  bool operator==(const SecondaryIndexRow &opj)
  {
    return (strcmp(Key, opj.Key) == 0);
  }
};

template <class T>
class SecondaryIndex
{
private:
  string filePath;

  void writeOnFile(vector<SecondaryIndexRow<T>> v)
  {
    fstream file;
    file.open(filePath, ios::out);
    for (SecondaryIndexRow item : v)
    {
      file.write((char *)&item, sizeof(item));
    }
    file.close();
  }

public:
  SecondaryIndex() {};
  SecondaryIndex(string filePath)
  {
    this->filePath = filePath;
  }

  vector<SecondaryIndexRow<T>> getVector()
  {
    vector<SecondaryIndexRow<T>> v;
    fstream file;
    file.open(filePath, ios::in);
    SecondaryIndexRow<T> s;
    while (file.read((char *)&s, sizeof(s)))
    {
      v.push_back(s);
    }
    file.close();
    return v;
  }

  void update_index(T key, char id[])
  {
    fstream file;
    file.open(filePath, ios::out | ios::app);
    file.seekp(0, ios::end);
    SecondaryIndexRow<T> studentIndex;
    strcpy(studentIndex.Id, id);
    strcpy(studentIndex.Key, key);
    file.write((char *)&studentIndex, sizeof(studentIndex));
    file.close();

    sortIndex();
  }

  void sortIndex()
  {
    vector<SecondaryIndexRow<T>> v = getVector();
    sort(v.begin(), v.end());
    writeOnFile(v);
  }

  void getID(T key, char *id)
  {
    fstream file;
    file.open(filePath);
    file.seekg(0, ios::end);
    int fileSize = file.tellg();
    file.seekg(0, ios::beg);

    int left = 0, right = fileSize, mid;
    SecondaryIndexRow<T> s;
    while (left <= right)
    {
      mid = left + right / 2;
      mid -= mid % sizeof(SecondaryIndexRow<T>);
      // cout << "left " << left << " mid " << mid << " right " << right << endl;
      file.seekg(mid, ios::beg);
      file.read((char *)&s, sizeof(s));
      // cout << s.id << " " << s.RRN << endl;
      if (strcmp(s.Key, key) == 0)
      {
        // id = s.Id;
        int idSize = sizeof(s.Id) / sizeof(char);
        for (int i = 0; i < idSize; i++)
        {
          id[i] = s.Id[i];
        }
        return;
      }
      else if (strcmp(s.Key, key) == -1)
      {
        left = file.tellg();
      }
      else
      {
        right = mid - sizeof(SecondaryIndexRow<T>);
      }
    }
    id = nullptr;
    return;
  }

  bool deleteKey(T key)
  {
    char id[15];
    getID(key, id);
    if (id != nullptr)
    {
      const SecondaryIndexRow<T> row(key, id);
      vector<SecondaryIndexRow<T>> v = getVector();
      // auto pos = binary_search(v.begin(), v.end(), row);

      // int it = find(v.begin(), v.end(), row) - v.begin();
      int it = find_if(v.begin(), v.end(), [row](SecondaryIndexRow<T> elem)
                       { return (strcmp(row.Key, elem.Key) == 0) && (strcmp(row.Id, elem.Id) == 0) ;}) -
               v.begin();
      // auto it = find_if(v.begin(), v.end(), [id](StudentSecondaryRow a){return a.id == id;});
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
