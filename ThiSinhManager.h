#ifndef THISINHMANAGER_H
#define THISINHMANAGER_H
#include "HashTable.h"
#include <string>

ThiSinh nhapThiSinh();                                   
bool themThiSinh(HashTable& ht, const ThiSinh& ts);      
Node* timTheoMa(const HashTable& ht, const std::string& ma);
int timTheoTen(const HashTable& ht, const std::string& ten);
void inThongTin(const ThiSinh& ts);
void hienThiDanhSach(const HashTable& ht);

#endif