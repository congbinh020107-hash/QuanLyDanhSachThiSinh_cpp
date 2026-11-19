#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <iostream>

const int NUM_BUCKETS = 26;

struct ThiSinh {
    std::string maDuThi;
    std::string hoTen;
    std::string gioiTinh;
    std::string ngaySinh;
    std::string queQuan;
};

struct Node {
    ThiSinh data;
    Node* next;
};

struct HashTable {
    Node* buckets[NUM_BUCKETS];
    int totalCount;
};

std::string trimString(const std::string& str);
std::string extractTenChinh(const std::string& hoTen);
char normalizeVietnameseFirstChar(const std::string& ten);
int hashFunction(const std::string& hoTen);
char bucketIndexToChar(int index);

void initHashTable(HashTable& ht);
Node* createNode(const ThiSinh& ts);
bool isExistMaDuThi(const HashTable& ht, const std::string& maDuThi);
bool insertNode(HashTable& ht, const ThiSinh& ts);
bool removeNodeByID(HashTable& ht, const std::string& maDuThi);
int countNodesInBucket(const HashTable& ht, int bucketIndex);
void clearHashTable(HashTable& ht);

#endif