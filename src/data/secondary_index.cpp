#ifndef SECONDARYINDEX
#define SECONDARYINDEX
#include "linked_list.cpp"

#include "bits/stdc++.h"
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

  // void writeOnFile(vector<SecondaryIndexRow<T>> v)
  // {
  //   fstream file;
  //   file.open(filePath, ios::out);
  //   for (SecondaryIndexRow item : v)
  //   {
  //     file.write((char *)&item, sizeof(item));
  //   }
  //   file.close();
  // }

public:
  SecondaryIndex() {};
  SecondaryIndex(string filePath, string linkedListPath)
  {
    this->filePath = filePath;
    list = LinkedList(linkedListPath);
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
    list.getVector();

    return v;
  }

  void update_index(const char* key, const char* id)
  {
    fstream file;
    // check if the key is in the secondary index
    long long positionInFile = this->getByteOffset(key);
    cout << "positionInFile: " << positionInFile << endl;
    if (positionInFile == -1)
    {
      // If the key is not in the index, add it to the secondary index
      file.open(filePath, ios::out | ios::app);
      file.seekp(0, ios::end);
      SecondaryIndexRow<T> studentIndex;
      int lastRRN = list.update_list(-1, id);
      cout << "lastRRN: " << lastRRN << endl;
      studentIndex.RRN= lastRRN;
      cout << "student.rrn: " << studentIndex.RRN << endl;
      strcpy(studentIndex.Key, key);
      file.write((char *)&studentIndex, sizeof(studentIndex));
      file.close();
      return;
    }
    else
    {
      // If the key is in the index, take the RRN from that position and update the linked list
      file.open(filePath, ios::in | ios::out); 
      if (!file.is_open()) {
          cerr << "Error: Unable to open file for reading and writing." << endl;
          return;
      }

      // Move to the the present record position in the file
      file.seekg(positionInFile, ios::beg);
      SecondaryIndexRow<T> studentIndex;

      // Read the existing record
      file.read((char *)&studentIndex, sizeof(studentIndex));

      // send the rrn of the record (in secondary index) to the linked list to add it
      int lastRRN = list.update_list(studentIndex.RRN, id);

      // Move back to the correct position to overwrite the record with the updated RRN
      file.seekp(positionInFile, ios::beg);
      studentIndex.RRN = lastRRN;

      // Write the updated record back to the file
      file.write((char *)&studentIndex, sizeof(studentIndex));

      // Debug output
      cout << "Updated and wrote to file: RRN = " << studentIndex.RRN << ", Key = " << studentIndex.Key << endl;

      file.close();

    }

    // sortIndex();
  }

  void sortIndex()
  {
    vector<SecondaryIndexRow<T>> v = getVector();
    sort(v.begin(), v.end());
    writeOnFile(v);
  }
  long long getByteOffset(const char* id) {
      fstream file;
      cout << "Opening file: " << filePath << endl;

      file.open(filePath, ios::in | ios::binary);
      if (!file.is_open()) {
          cerr << "Error: Could not open file " << filePath << endl;
          return -1;
      }

      file.seekg(0, ios::end);
      long long fileSize = file.tellg();
      cout << "File size: " << fileSize << " bytes" << endl;

      if (fileSize == 0) {
          cout << "File is empty." << endl;
          file.close();
          return -1; // Empty file
      }

      file.seekg(0, ios::beg);

      long long left = 0, right = fileSize, mid;
      SecondaryIndexRow<T> s;

      while (left <= right) {
          mid = left + (right - left) / 2;
          mid -= mid % sizeof(SecondaryIndexRow<T>); // Align to record size

          cout << "Left: " << left << ", Mid: " << mid << ", Right: " << right << endl;

          file.seekg(mid, ios::beg);
          file.read((char*)&s, sizeof(s));

          cout << "Read record - Key: " << s.Key << ", RRN: " << s.RRN << endl;

          int cmp = strcmp(s.Key, id);
          if (cmp == 0) {
              cout << "Found key '" << id << "' at byte offset: " << mid << endl;
              file.close();
              return mid; // Found the record
          } else if (cmp < 0) { // s.Key < id
              cout << "Key '" << s.Key << "' is less than '" << id << "'. Moving right." << endl;
              left = mid + sizeof(SecondaryIndexRow<T>);
          } else { // s.Key > id
              cout << "Key '" << s.Key << "' is greater than '" << id << "'. Moving left." << endl;
              right = mid - sizeof(SecondaryIndexRow<T>);
          }
      }

      cout << "Key '" << id << "' not found in the file." << endl;
      file.close();
      return -1; // Not found
  }


 
  // void getID(T key, char *id)
  // {
  //   fstream file;
  //   file.open(filePath);
  //   file.seekg(0, ios::end);
  //   int fileSize = file.tellg();
  //   file.seekg(0, ios::beg);

  //   int left = 0, right = fileSize, mid;
  //   SecondaryIndexRow<T> s;
  //   while (left <= right)
  //   {
  //     mid = left + right / 2;
  //     mid -= mid % sizeof(SecondaryIndexRow<T>);
  //     // cout << "left " << left << " mid " << mid << " right " << right << endl;
  //     file.seekg(mid, ios::beg);
  //     file.read((char *)&s, sizeof(s));
  //     // cout << s.id << " " << s.RRN << endl;
  //     if (strcmp(s.Key, key) == 0)
  //     {
  //       // RRN = s.Id;
  //       int RRNSize = sizeof(s.Id) / sizeof(char);
  //       for (int i = 0; i < RRNSize; i++)
  //       {
  //         RRN[i] = s.Id[i];
  //       }
  //       return;
  //     }
  //     else if (strcmp(s.Key, key) == -1)
  //     {
  //       left = file.tellg();
  //     }
  //     else
  //     {
  //       right = mid - sizeof(SecondaryIndexRow<T>);
  //     }
  //   }
  //   RRN = nullptr;
  //   return;
  // }

  // bool deleteKey(T key)
  // {
  //   char RRN[15];
  //   getID(key, RRN);
  //   if (id != nullptr)
  //   {
  //     const SecondaryIndexRow<T> row(key, RRN);
  //     vector<SecondaryIndexRow<T>> v = getVector();
  //     // auto pos = binary_search(v.begin(), v.end(), row);

  //     // int it = find(v.begin(), v.end(), row) - v.begin();
  //     int it = find_if(v.begin(), v.end(), [row](SecondaryIndexRow<T> elem)
  //                      { return (strcmp(row.Key, elem.Key) == 0) && (strcmp(row.Id, elem.Id) == 0) ;}) -
  //              v.begin();
  //     // auto it = find_if(v.begin(), v.end(), [id](StudentSecondaryRow a){return a.id == RRN;});
  //     // int pos = it - v.begin();

  //     // cout << "id: " << RRN << " RRN: " << byteOffset << " pos: " << pos << endl;
  //     v.erase(v.begin() + it);
  //     writeOnFile(v);
  //     return true;
  //   }
  //   return false;
  // }
};
#endif
