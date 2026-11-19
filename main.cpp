#include "HashTable.h"
#include "FileManager.h"
#include "ThiSinhManager.h"
#include "Algorithm.h"
#include "Test.h"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

void nhapChuoi(const string& thongBao, string& ketQua) {
    cout << thongBao;
    getline(cin, ketQua);
}

void nhapSoPhong(int& soPhong) {
    cout << "Nhap so phong thi: ";
    cin >> soPhong;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void xuatMenu() {
    cout << "\n========== QUAN LY THI SINH ==========\n";
    cout << "1. Nap du lieu tu file\n";
    cout << "2. Them thi sinh\n";
    cout << "3. Tim thi sinh theo ma du thi\n";
    cout << "4. Tim thi sinh theo ten\n";
    cout << "5. Hien thi danh sach thi sinh\n";
    cout << "6. Xoa thi sinh theo ma du thi\n";
    cout << "7. Sap xep thi sinh theo ten\n";
    cout << "8. Phan thi sinh vao cac phong thi\n";
    cout << "9. Luu du lieu ra file\n";
    cout << "10. Chay cac test\n";
    cout << "0. Thoat chuong trinh\n";
    cout << "======================================\n";
    cout << "Lua chon: ";
}

void layDanhSachTuHashTable(
    const HashTable& ht,
    vector<ThiSinh>& danhSach
) {
    danhSach.clear();

    for (int i = 0; i < NUM_BUCKETS; ++i) {
        Node* hienTai = ht.buckets[i];

        while (hienTai != nullptr) {
            danhSach.push_back(hienTai->data);
            hienTai = hienTai->next;
        }
    }
}

int main() {
    HashTable ht;
    initHashTable(ht);

    const string duongDanFile = "DATA/thisinh.txt";

    LoadResult kqNap;
    loadFromFile(ht, duongDanFile, &kqNap);

    int luaChon = -1;

    do {
        xuatMenu();

        if (!(cin >> luaChon)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Lua chon khong hop le.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (luaChon) {
            case 1: {
                string duongDan;
                nhapChuoi(
                    "Nhap duong dan file: ",
                    duongDan
                );

                if (duongDan.empty()) {
                    duongDan = duongDanFile;
                }

                LoadResult ketQua;
                loadFromFile(ht, duongDan, &ketQua);

                if (ketQua.moDuocFile) {
                    cout << "\nKet qua nap file:\n";
                    cout << "- Dong du lieu: "
                         << ketQua.tongSoDong << '\n';
                    cout << "- Nap thanh cong: "
                         << ketQua.soThanhCong << '\n';
                    cout << "- Sai dinh dang: "
                         << ketQua.soSaiDinhDang << '\n';
                    cout << "- Trung ma du thi: "
                         << ketQua.soTrungMa << '\n';
                }
                break;
            }

            case 2: {
                ThiSinh ts = nhapThiSinh();
                themThiSinh(ht, ts);
                break;
            }

            case 3: {
                string ma;
                nhapChuoi(
                    "Nhap ma du thi can tim: ",
                    ma
                );

                Node* ketQua = timTheoMa(ht, ma);

                if (ketQua == nullptr) {
                    cout << "Khong tim thay thi sinh.\n";
                } else {
                    cout << "Tim thay:\n";
                    inThongTin(ketQua->data);
                }
                break;
            }

            case 4: {
                string ten;
                nhapChuoi(
                    "Nhap ten can tim: ",
                    ten
                );

                int soKetQua = timTheoTen(ht, ten);

                if (soKetQua == 0) {
                    cout << "Khong tim thay thi sinh.\n";
                } else {
                    cout << "Tim thay " << soKetQua
                         << " thi sinh.\n";
                }
                break;
            }

            case 5:
                hienThiDanhSach(ht);
                break;

            case 6: {
                string ma;
                nhapChuoi(
                    "Nhap ma du thi can xoa: ",
                    ma
                );

                if (removeNodeByID(ht, ma)) {
                    cout << "Da xoa thi sinh co ma "
                         << trimString(ma) << ".\n";
                } else {
                    cout << "Khong tim thay ma du thi "
                         << trimString(ma) << ".\n";
                }
                break;
            }

            case 7: {
                vector<ThiSinh> danhSach;
                layDanhSachTuHashTable(ht, danhSach);

                sapXepTheoTen(
                    danhSach,
                    [](const ThiSinh& ts) {
                        // Sắp xếp theo tên chính.
                        return extractTenChinh(ts.hoTen);
                    }
                );

                cout << "\n=== DANH SACH SAP XEP THEO TEN ===\n";

                if (danhSach.empty()) {
                    cout << "Danh sach rong.\n";
                    break;
                }

                for (size_t i = 0; i < danhSach.size(); ++i) {
                    cout << i + 1 << ". ";
                    inThongTin(danhSach[i]);
                }
                break;
            }

            case 8: {
                int soPhong;
                nhapSoPhong(soPhong);

                vector<ThiSinh> danhSach;
                layDanhSachTuHashTable(ht, danhSach);

                vector<vector<ThiSinh> > cacPhong;

                if (!chiaThiSinhVaoPhong(
                        danhSach,
                        soPhong,
                        cacPhong
                    )) {
                    cout << "So phong phai lon hon 0.\n";
                    break;
                }

                if (danhSach.empty()) {
                    cout << "Danh sach thi sinh rong.\n";
                    break;
                }

                cout << "\n=== PHAN PHONG THI ===\n";

                for (size_t i = 0; i < cacPhong.size(); ++i) {
                    cout << "\nPhong " << i + 1
                         << " (" << cacPhong[i].size()
                         << " thi sinh):\n";

                    if (cacPhong[i].empty()) {
                        cout << "  (Trong)\n";
                        continue;
                    }

                    for (size_t j = 0;
                         j < cacPhong[i].size();
                         ++j) {
                        cout << "  " << j + 1 << ". ";
                        cout << "[" << cacPhong[i][j].maDuThi << "] "
                             << cacPhong[i][j].hoTen << '\n';
                    }
                }
                break;
            }

            case 9: {
                string duongDan;
                nhapChuoi(
                    "Nhap duong dan file can ghi: ",
                    duongDan
                );

                if (duongDan.empty()) {
                    duongDan = duongDanFile;
                }

                if (saveToFile(ht, duongDan)) {
                    cout << "Da luu du lieu thanh cong.\n";
                }
                break;
            }

            case 10:
                chayTatCaTest();
                break;

            case 0:
                cout << "Da thoat chuong trinh.\n";
                break;

            default:
                cout << "Lua chon khong ton tai.\n";
                break;
        }

    } while (luaChon != 0);

    clearHashTable(ht);
    return 0;
}