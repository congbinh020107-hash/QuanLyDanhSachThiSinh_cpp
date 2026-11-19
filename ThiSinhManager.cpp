#include "ThiSinhManager.h"
#include <iostream>
using namespace std;

static string chuThuong(string s) {
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] + 32;
        }
    }
    return s;
}
ThiSinh nhapThiSinh() {
    ThiSinh ts;
    cout << "Ma du thi: ";              getline(cin, ts.maDuThi);
    cout << "Ho ten: ";                 getline(cin, ts.hoTen);
    cout << "Gioi tinh: ";              getline(cin, ts.gioiTinh);
    cout << "Ngay sinh (dd/mm/yyyy): "; getline(cin, ts.ngaySinh);
    cout << "Que quan: ";               getline(cin, ts.queQuan);
    return ts;
}
bool themThiSinh(HashTable& ht, const ThiSinh& ts) {
    string ma = trimString(ts.maDuThi);
    string ten = trimString(ts.hoTen);
    if (ma.empty() || ten.empty()) {
        cout << "Them that bai: ma du thi va ho ten khong duoc de trong" << endl;
        return false;
    }
    if (isExistMaDuThi(ht, ma)) {
        cout << "Them that bai: trung ma du thi " << ma << endl;
        return false;
    }
    insertNode(ht, ts);
    cout << "Them thanh cong thi sinh " << ma << endl;
    return true;
}
Node* timTheoMa(const HashTable& ht, const string& ma) {
    string maTim = chuThuong(trimString(ma));
    if (maTim.empty()) return nullptr;

    for (int i = 0; i < NUM_BUCKETS; i++) {
        Node* p = ht.buckets[i];
        while (p != nullptr) {
            if (chuThuong(p->data.maDuThi) == maTim) {
                return p;
            }
            p = p->next;
        }
    }
    return nullptr;
}
int timTheoTen(const HashTable& ht, const string& ten) {
    string tenTim = chuThuong(trimString(ten));
    if (tenTim.empty()) return 0;
    bool hoTenDayDu = (tenTim.find(' ') != string::npos);
    int index = hashFunction(ten);   
    int dem = 0;
    Node* p = ht.buckets[index];
    while (p != nullptr) {
        bool khop;
        if (hoTenDayDu) {
            khop = (chuThuong(p->data.hoTen) == tenTim);
        }
        else {
            khop = (chuThuong(extractTenChinh(p->data.hoTen)) == tenTim);
        }
        if (khop) {
            inThongTin(p->data);
            dem++;
        }
        p = p->next;
    }
    return dem;
}
void inThongTin(const ThiSinh& ts) {
    cout << "  - [" << ts.maDuThi << "] " << ts.hoTen
        << " | " << ts.gioiTinh
        << " | " << ts.ngaySinh
        << " | Que: " << ts.queQuan << endl;
}
void hienThiDanhSach(const HashTable& ht) {
    if (ht.totalCount == 0) {
        cout << "Danh sach thi sinh rong" << endl;
        return;
    }
    cout << "=== DANH SACH THI SINH ===" << endl;
    for (int i = 0; i < NUM_BUCKETS; i++) {
        if (ht.buckets[i] == nullptr) continue;
        cout << "Bucket " << bucketIndexToChar(i) << ":" << endl;
        Node* p = ht.buckets[i];
        while (p != nullptr) {
            inThongTin(p->data);
            p = p->next;
        }
    }
    cout << "Tong so: " << ht.totalCount << endl;
}