#include "HashTable.h"
#include <cctype>
#include <sstream>
#include <vector>

std::string trimString(const std::string& str) {
    std::istringstream iss(str);
    std::string word, result;
    while (iss >> word) {
        if (!result.empty()) result += " ";
        result += word;
    }
    return result;
}

std::string extractTenChinh(const std::string& hoTen) {
    std::string cleaned = trimString(hoTen);
    if (cleaned.empty()) return "";

    size_t lastSpace = cleaned.find_last_of(' ');
    if (lastSpace == std::string::npos) {
        return cleaned;
    }
    return cleaned.substr(lastSpace + 1);
}

char normalizeVietnameseFirstChar(const std::string& ten) {
    if (ten.empty()) return 'A';

    unsigned char firstByte = static_cast<unsigned char>(ten[0]);
    if (firstByte < 128) {
        if (std::isalpha(firstByte)) {
            return static_cast<char>(std::toupper(firstByte));
        }
        return 'A';
    }

    struct Utf8Map {
        char asciiChar;
        std::vector<std::string> prefixes;
    };

    static const Utf8Map vnMappings[] = {
        {'A', {"À","Á","Ả","Ã","Ạ","à","á","ả","ã","ạ",
               "Ă","Ằ","Ắ","Ẳ","Ẵ","Ặ","ă","ằ","ắ","ẳ","ẵ","ặ",
               "Â","Ầ","Ấ","Ẩ","Ẫ","Ậ","â","ầ","ấ","ẩ","ẫ","ậ"}},
        {'D', {"Đ","đ"}},
        {'E', {"È","É","Ẻ","Ẽ","Ẹ","è","é","ẻ","ẽ","ẹ",
               "Ê","Ề","Ế","Ể","Ễ","Ệ","ê","ề","ế","ể","ễ","ệ"}},
        {'I', {"Ì","Í","Ỉ","Ĩ","Ị","ì","í","ỉ","ĩ","ị"}},
        {'O', {"Ò","Ó","Ỏ","Õ","Ọ","ò","ó","ỏ","õ","ọ",
               "Ô","Ồ","Ố","Ổ","Ỗ","Ộ","ô","ồ","ố","ổ","ỗ","ộ",
               "Ơ","Ờ","Ớ","Ở","Ỡ","Ợ","ơ","ờ","ớ","ở","ỡ","ợ"}},
        {'U', {"Ù","Ú","Ủ","Ũ","Ụ","ù","ú","ủ","ũ","ụ",
               "Ư","Ừ","Ứ","Ử","Ữ","Ự","ư","ừ","ứ","ử","ữ","ự"}},
        {'Y', {"Ỳ","Ý","Ỷ","Ỹ","Ỵ","ỳ","ý","ỷ","ỹ","ỵ"}}
    };

    for (const auto& group : vnMappings) {
        for (const auto& prefix : group.prefixes) {
            if (ten.compare(0, prefix.size(), prefix) == 0) {
                return group.asciiChar;
            }
        }
    }

    return 'A';
}

int hashFunction(const std::string& hoTen) {
    std::string tenChinh = extractTenChinh(hoTen);
    char firstLetter = normalizeVietnameseFirstChar(tenChinh);

    if (firstLetter >= 'A' && firstLetter <= 'Z') {
        return firstLetter - 'A';
    }
    return 0;
}

char bucketIndexToChar(int index) {
    if (index >= 0 && index < NUM_BUCKETS) {
        return static_cast<char>('A' + index);
    }
    return '?';
}

void initHashTable(HashTable& ht) {
    for (int i = 0; i < NUM_BUCKETS; ++i) {
        ht.buckets[i] = nullptr;
    }
    ht.totalCount = 0;
}

Node* createNode(const ThiSinh& ts) {
    Node* newNode = new Node;
    newNode->data.maDuThi  = trimString(ts.maDuThi);
    newNode->data.hoTen    = trimString(ts.hoTen);
    newNode->data.gioiTinh = trimString(ts.gioiTinh);
    newNode->data.ngaySinh = trimString(ts.ngaySinh);
    newNode->data.queQuan  = trimString(ts.queQuan);
    newNode->next = nullptr;
    return newNode;
}

bool isExistMaDuThi(const HashTable& ht, const std::string& maDuThi) {
    std::string cleanID = trimString(maDuThi);
    if (cleanID.empty()) return false;

    for (int i = 0; i < NUM_BUCKETS; ++i) {
        Node* curr = ht.buckets[i];
        while (curr != nullptr) {
            if (curr->data.maDuThi == cleanID) {
                return true;
            }
            curr = curr->next;
        }
    }
    return false;
}

bool insertNode(HashTable& ht, const ThiSinh& ts) {
    std::string cleanID = trimString(ts.maDuThi);
    std::string cleanName = trimString(ts.hoTen);

    if (cleanID.empty() || cleanName.empty() || isExistMaDuThi(ht, cleanID)) {
        return false;
    }

    int index = hashFunction(cleanName);
    Node* newNode = createNode(ts);

    if (ht.buckets[index] == nullptr) {
        ht.buckets[index] = newNode;
    } else {
        Node* temp = ht.buckets[index];
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    ht.totalCount++;
    return true;
}

bool removeNodeByID(HashTable& ht, const std::string& maDuThi) {
    std::string cleanID = trimString(maDuThi);
    if (cleanID.empty()) return false;

    for (int i = 0; i < NUM_BUCKETS; ++i) {
        Node* curr = ht.buckets[i];
        Node* prev = nullptr;

        while (curr != nullptr) {
            if (curr->data.maDuThi == cleanID) {
                if (prev == nullptr) {
                    ht.buckets[i] = curr->next;
                } else {
                    prev->next = curr->next;
                }
                delete curr;
                ht.totalCount--;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
    }
    return false;
}

int countNodesInBucket(const HashTable& ht, int bucketIndex) {
    if (bucketIndex < 0 || bucketIndex >= NUM_BUCKETS) return 0;

    int count = 0;
    Node* curr = ht.buckets[bucketIndex];
    while (curr != nullptr) {
        count++;
        curr = curr->next;
    }
    return count;
}

void clearHashTable(HashTable& ht) {
    for (int i = 0; i < NUM_BUCKETS; ++i) {
        Node* curr = ht.buckets[i];
        while (curr != nullptr) {
            Node* toDelete = curr;
            curr = curr->next;
            delete toDelete;
        }
        ht.buckets[i] = nullptr;
    }
    ht.totalCount = 0;
}