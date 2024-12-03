#ifndef LINKEDLIST
#define LINKEDLIST

#include "bits/stdc++.h"
using namespace std;

struct LinkedListRow
{
  int RRN;
  char Id[15];
  LinkedListRow() {};
  LinkedListRow(char id[], int rrn)
  {
    strcpy(Id,id);
    this->RRN = rrn;
  };

 
};


class LinkedList
{
private:
  string filePath;

  void writeOnFile(vector<LinkedListRow> v)
  {
    fstream file;
    file.open(filePath, ios::out);
    for (LinkedListRow item : v)
    {
      file.write((char *)&item, sizeof(item));
    }
    file.close();
  }

public:
  LinkedList() {};
  LinkedList (string filePath)
  {
    this->filePath = filePath;
  }

  vector<LinkedListRow> getVector()
  {
    vector<LinkedListRow> v;
    fstream file;
    file.open(filePath, ios::in);
    LinkedListRow s;
    while (file.read((char *)&s, sizeof(s)))
    {
      v.push_back(s);
      cout << s.Id << " " << s.RRN << endl;
    }
    file.close();
    return v;
  }

  int update_list(int RRN, const char* id)
  {
    fstream file;
    file.open(filePath, ios::out | ios::app);
    // writes at the end of the file
    file.seekp(0, ios::end);

    LinkedListRow studentLinkedList;
    // copies id and RRN to the struct
    strcpy(studentLinkedList.Id, id);
    studentLinkedList.RRN = RRN;

    // writes the struct to the file
    file.write((char *)&studentLinkedList, sizeof(studentLinkedList));
    
    // returns the RRN of this inserted record in the linkedlist
    int RecordRRN = (file.tellp() / sizeof(LinkedListRow)) - 1;
    cout << "RecordRRN: " << RecordRRN << endl;
    file.close();
    return RecordRRN; 
  }


  // bool deleteRRN(int RRN)
  // {

  // }
};
#endif
