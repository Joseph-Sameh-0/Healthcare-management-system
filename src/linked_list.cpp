#ifndef LINKEDLIST
#define LINKEDLIST

// #include "bits/stdc++.h"
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <algorithm>
#include <fstream>
using namespace std;

struct LinkedListRow
{
  char Id[15];
  int RRN;
  LinkedListRow() {};
  LinkedListRow(char id[], int rrn)
  {
    strcpy(Id, id);
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

  void deleteRow(long long RRN)
  {
    const char c[15] = {'#'};
    LinkedListRow row;
    strcpy(row.Id, c);
    row.RRN = -1;

    overrideRow(row, RRN);
  }

  void overrideRow(LinkedListRow row, long long RRN)
  {
    fstream file;
    file.open(filePath);
    file.seekp(RRN * sizeof(LinkedListRow), ios::beg);
    file.write((char *)&row, sizeof(row));
    file.close();
  }

  vector<LinkedListRow> getVector()
  {
    vector<LinkedListRow> v;
    fstream file;
    file.open(filePath, ios::in);
    LinkedListRow s;
    while (file.read((char *)&s, sizeof(s)))
    {
      if (s.Id[0] != '#')
      {
        v.push_back(s);
      }
      // cout << s.Id << " " << s.RRN << endl;
    }
    file.close();
    return v;
  }

  vector<LinkedListRow> getVector(long long firstRRN)
  {
    vector<LinkedListRow> v;
    fstream file;
    file.open(filePath);
    long long readRRN = firstRRN;
    LinkedListRow s;
    do
    {
      int position = readRRN * sizeof(LinkedListRow);
      file.seekg(position, ios::beg);

      file.read((char *)&s, sizeof(s));
      if (s.Id[0] != '#')
      {
        v.push_back(s);
      }
      readRRN = s.RRN; // read the next RRN
    } while (s.RRN != -1);
    file.close();

    return v;
  }

public:
  LinkedList() {};
  LinkedList(string filePath)
  {
    this->filePath = filePath;
    fstream file;
    file.open(filePath, ios::out | ios::app);
    file.close();
  }

  int add_to_list(const char *id, int RRN)
  {
    fstream file;
    file.open(filePath, ios::in | ios::out | ios::app);

    if (RRN != -1) // if RRN is -1 (not first record), check if it exists
    {
      long long readRRN = RRN;

      LinkedListRow s;
      do
      {
        file.seekg(readRRN * sizeof(LinkedListRow), ios::beg);
        file.read((char *)&s, sizeof(s));
        if (strcmp(s.Id, id) == 0)
        {
          cout << "id " + string(id) + " already exists" << endl;
          return RRN; // id already exists
        }
        readRRN = s.RRN; // read the next RRN
      } while (s.RRN != -1);
    }

    // writes at the end of the file
    file.seekp(0, ios::end);

    LinkedListRow studentLinkedList;
    // copies id and RRN to the struct
    strcpy(studentLinkedList.Id, id);
    studentLinkedList.RRN = RRN;

    // writes the struct to the file
    file.write((char *)&studentLinkedList, sizeof(studentLinkedList));
    // cout << "id rrn " << studentLinkedList.Id << "  " << studentLinkedList.RRN << endl;
    // returns the RRN of this inserted record in the linkedlist
    int RecordRRN = (file.tellp() / sizeof(LinkedListRow)) - 1;
    // cout << "RecordRRN: " << RecordRRN << endl;
    file.close();
    return RecordRRN;
  }

  int deleteID(const char *id, int firstRRNOfKey)
  {
    vector<LinkedListRow> v = getVector(firstRRNOfKey);

    // find the actual rrn of the id to be deleted
    int rowIndex = -1;
    for (size_t i = 0; i < v.size(); i++)
    {
      if (strcmp(v[i].Id, id) == 0)
      {
        rowIndex = i; // index of the record to be deleted
        break;
      }
    }

    if (rowIndex == -1) // if the id is not found
    {
      cout << "ID not found" << endl;
      return -2;
    }

    if (v.size() == 1)
    {
      deleteRow(firstRRNOfKey); // delete the only record
      return -1;
    }

    if (rowIndex == 0) // if the id to be deleted is the first record
    {
      //  delete the first record
      deleteRow(firstRRNOfKey);
      // return the next RRN
      return v[0].RRN;
    }

    long long rowRRN = v[rowIndex - 1].RRN; // get the actual RRN of the record
    if (rowIndex == (int)(v.size() - 1))         // if the id to be deleted is the last record
    {
      // delete the last record
      deleteRow(rowRRN);

      // update the prev RRN of the second last record to -1
      LinkedListRow lastRecord = v[rowIndex - 1];
      lastRecord.RRN = -1; // update the prev RRN of the second last record to -1

      if (v.size() > 2) // if there are more than 2 records
      {
        overrideRow(lastRecord, v[rowIndex - 2].RRN); //  override the last record with -1 RRN
      }
      else
      {
        overrideRow(lastRecord, firstRRNOfKey); // override the last record with -1 RRN
      }
    }
    else //  if the id to be deleted is in the middle
    {
      // delete the middle record
      deleteRow(rowRRN);

      // update the prev RRN of the previous record to the RRN of the next record
      LinkedListRow prevRecord = v[rowIndex - 1]; // get the previous record
      prevRecord.RRN = v[rowIndex].RRN;           // update the next RRN of the next record to the RRN of the previous record

      if (rowIndex == 1)
      {
        overrideRow(prevRecord, firstRRNOfKey);
      }
      else
      {
        overrideRow(prevRecord, v[rowIndex - 2].RRN);
      }
    }

    // clean the file
    // clean();
    return -2;
  }

  vector<string> getIds(long long firstRRNOfKey)
  {
    vector<string> ids;
    fstream file;
    file.open(filePath, ios::in);
    long long readRRN = firstRRNOfKey;

    LinkedListRow s;
    do
    {
      file.seekg(readRRN * sizeof(LinkedListRow), ios::beg);
      file.read((char *)&s, sizeof(s));
      // const char *id = s.Id;
      // id = string(s.Id);
      ids.push_back(s.Id);
      readRRN = s.RRN;
    } while (s.RRN != -1);
    file.close();
    return ids;
  }

  void deleteAll(long long firstRRNOfKey)
  {
    fstream file;
    file.open(filePath, ios::in | ios::out);

    long long readRRN = firstRRNOfKey;
    LinkedListRow s;
    do
    {
      file.seekg(readRRN * sizeof(LinkedListRow), ios::beg);
      file.read((char *)&s, sizeof(s));
      strcpy(s.Id, "#");
      overrideRow(s, readRRN);
      readRRN = s.RRN; // update the RRN to the next record
    } while (s.RRN != -1);

    // clean();
    file.close();
  }

  bool edit(const char *oldId, const char *newId, long long firstRRNOfKey)
  {
    fstream file;
    file.open(filePath, ios::in | ios::out | ios::app);

    long long readRRN = firstRRNOfKey;

    LinkedListRow s;
    do
    {
      file.seekg(readRRN * sizeof(LinkedListRow), ios::beg);
      file.read((char *)&s, sizeof(s));
      if (strcmp(s.Id, oldId) == 0)
      {
        strcpy(s.Id, newId); // update the id
        overrideRow(s, readRRN);
        return true;
      }
      readRRN = s.RRN; // read the next RRN
    } while (s.RRN != -1);

    return false;
  }
};
#endif
