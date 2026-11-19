#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <string>
#include "HashTable.h"   
struct LoadResult {
    bool moDuocFile;      
    int  tongSoDong;     
    int  soThanhCong;     
    int  soSaiDinhDang;   
    int  soTrungMa;       
};

bool loadFromFile(HashTable& ht, const std::string& filePath, LoadResult* kq = nullptr);

bool saveToFile(const HashTable& ht, const std::string& filePath);

#endif
