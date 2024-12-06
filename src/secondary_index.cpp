#ifndef SECONDARYINDEX
#define SECONDARYINDEX

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <string.h>
#include "linked_list.cpp"

using namespace std;

template <class T>
struct SecondaryIndexRow
{
  T Key;
  int RRN;
  SecondaryIndexRow() {};
  SecondaryIndexRow(T key, int RRN)
  {
    this->RRN = RRN;
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
  LinkedList list;

  void writeOnFile(vector<SecondaryIndexRow<T>> v)
  {
    fstream file;
    file.open(filePath, ios::out);
    for (SecondaryIndexRow<T> item : v)
    {
      file.write((char *)&item, sizeof(item));
    }
    file.close();
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

    // list.getVector(); //
    return v;
  }

  long long getPosition(const char *key)
  {
    fstream file;
    file.open(filePath);
    file.seekg(0, ios::end);
    int fileSize = file.tellg();
    file.seekg(0, ios::beg);

    if (fileSize == 0)
      return -1;

    int left = 0, right = fileSize, mid;
    SecondaryIndexRow<T> s;
    while (left <= right)
    {
      mid = left + (right - left) / 2;
      mid -= mid % sizeof(SecondaryIndexRow<T>);
      // cout << "left " << left << " mid " << mid << " right " << right << endl;
      file.seekg(mid, ios::beg);
      file.read((char *)&s, sizeof(s));
      // cout << s.id << " " << s.RRN << endl;
      int cmp = strcmp(s.Key, key);
      if (cmp == 0)
      {
        return mid;
      }
      else if (cmp == -1)
      {
        left = file.tellg();
        if (left >= fileSize)
          return -1;
      }
      else
      {
        right = mid - sizeof(SecondaryIndexRow<T>);
      }
    }
    return -1;
  }

  void sortIndex()
  {
    vector<SecondaryIndexRow<T>> v = getVector();
    sort(v.begin(), v.end());
    writeOnFile(v);
  }

public:
  SecondaryIndex() {};
  SecondaryIndex(string filePath, string linkedListPath)
  {
    this->filePath = filePath;
    list = LinkedList(linkedListPath);
    fstream file;
    file.open(filePath, ios::in | ios::out | ios::app);
    file.close();
  }

  void add(const char *key, const char *id)
  {
    long long position = getPosition(key);

    if (position == -1)
    {
      fstream file;
      file.open(filePath, ios::out | ios::app);
      file.seekp(0, ios::end);
      SecondaryIndexRow<T> studentIndex;
      strcpy(studentIndex.Key, key);
      studentIndex.RRN = list.add_to_list(id, -1);
      // cout << "key rrn " << studentIndex.Key << " " << studentIndex.RRN << endl; // debug
      file.write((char *)&studentIndex, sizeof(studentIndex));
      file.close();
      sortIndex();
    }
    else
    {
      fstream file;
      file.open(filePath, ios::in | ios::out);
      file.seekg(position, ios::beg); // seek to the record
      SecondaryIndexRow<T> studentIndex;
      file.read((char *)&studentIndex, sizeof(studentIndex)); // read the record
      // strcpy(studentIndex.Key, key);
      studentIndex.RRN = list.add_to_list(id, studentIndex.RRN); // update the RRN
      file.seekp(position, ios::beg);                            // seek to the record
      // cout << "key rrn:" << studentIndex.Key << " " << studentIndex.RRN << endl; // debug
      file.write((char *)&studentIndex, sizeof(studentIndex)); // write the record
      file.close();
    }
  }

  bool deleteKey(const char *key)
  {
    long long position = getPosition(key);
    if (position == -1)
    {
      cout << "Key not found" << endl;
      return false;
    };

    fstream file;
    file.open(filePath);
    file.seekg(position, ios::beg); // seek to the record
    SecondaryIndexRow<T> row;
    file.read((char *)&row, sizeof(row)); // read the record

    vector<SecondaryIndexRow<T>> v = getVector();
    int it = find_if(v.begin(), v.end(), [row](SecondaryIndexRow<T> elem)
                     { return (strcmp(row.Key, elem.Key) == 0) && (row.RRN == elem.RRN); }) -
             v.begin();

    v.erase(v.begin() + it);
    writeOnFile(v);
    if (row.RRN != -1)
    {
      list.deleteAll(row.RRN); // delete all the records with the given RRN
    }
    return true;
  }

  bool deleteId(const char *key, const char *id)
  {
    long long position = getPosition(key);
    if (position == -1)
    {
      cout << "Key not found" << endl;
      return false;
    }

    fstream file;
    file.open(filePath);
    file.seekg(position, ios::beg); // seek to the record
    SecondaryIndexRow<T> row;
    file.read((char *)&row, sizeof(row)); // read the record

    int newRRN = list.deleteID(id, row.RRN);
    if (newRRN != -2) // if the record was deleted
    {
      row.RRN = newRRN;
      file.seekp(position, ios::beg);        // seek to the record
      file.write((char *)&row, sizeof(row)); // write the record
      file.close();

      if (newRRN == -1) //  if the id is the last id
      {
        deleteKey(key);
        cout << "Name " << key << " deleted" << endl;
      }
    }
    cout << "Doctor " << key << " with  id " << id << " deleted" << endl;
    return true;
  }

  vector<string> getIds(const char *key)
  {
    vector<string> ids;
    long long position = getPosition(key);
    if (position == -1) // if the key is not found
    {
      return ids;
    } // return an empty vector
    fstream file;
    file.open(filePath, ios::in);
    file.seekg(position, ios::beg);
    SecondaryIndexRow<T> row;
    file.read((char *)&row, sizeof(row));
    ids = list.getIds(row.RRN); // get all the ids from the list
    return ids;
  }

  bool edit(const char *key, const char *oldId, const char *newId)
  {
    long long position = getPosition(key);
    if (position == -1) // key not found
    {
      cout << "key " << key << " not found" << endl;
      return false;
    }
    else
    {
      fstream file;
      file.open(filePath, ios::in | ios::out);
      file.seekg(position, ios::beg); // seek to the record
      SecondaryIndexRow<T> studentIndex;
      file.read((char *)&studentIndex, sizeof(studentIndex)); // read the record
      bool edited = list.edit(oldId, newId, studentIndex.RRN); // edit the id in the list
      file.close();
      return edited; // return true if the id is edited

    }
  }
};
#endif
