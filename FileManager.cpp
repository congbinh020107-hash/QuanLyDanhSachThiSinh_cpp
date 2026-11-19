#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

static const char PHAN_CACH = '|';

static const int SO_TRUONG = 5;

static const bool KIEM_TRA_NGAY_SINH = true;

static std::vector<std::string> tachTruong(const std::string& dong) {
    std::vector<std::string> ketQua;
    size_t batDau = 0;
    while (true) {
        size_t viTri = dong.find(PHAN_CACH, batDau);
        if (viTri == std::string::npos) {
            ketQua.push_back(dong.substr(batDau));   
            break;
        }
        ketQua.push_back(dong.substr(batDau, viTri - batDau));
        batDau = viTri + 1;
    }
    return ketQua;
}

static bool ngaySinhHopLe(const std::string& s) {
    if (s.empty()) return true;

    int ngay, thang, nam;
    char g1, g2;
    std::istringstream iss(s);
    if (!(iss >> ngay >> g1 >> thang >> g2 >> nam)) return false;
    if (g1 != '/' || g2 != '/') return false;

    char thua;                      
    if (iss >> thua) return false;

    if (nam < 1900 || nam > 2100) return false;
    if (thang < 1 || thang > 12) return false;

    int soNgayTrongThang[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool namNhuan = (nam % 400 == 0) || (nam % 4 == 0 && nam % 100 != 0);
    if (namNhuan) soNgayTrongThang[1] = 29;

    return ngay >= 1 && ngay <= soNgayTrongThang[thang - 1];
}

static std::string lamSachTruocKhiGhi(std::string s) {
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == PHAN_CACH) s[i] = ' ';
    }
    return s;
}

bool loadFromFile(HashTable& ht, const std::string& filePath, LoadResult* kq) {
    LoadResult ketQua = {false, 0, 0, 0, 0};

    std::ifstream file(filePath.c_str());
    if (!file.is_open()) {
        std::cout << "[LOI] Khong mo duoc file: " << filePath << "\n";
        if (kq) *kq = ketQua;
        return false;
    }
    ketQua.moDuocFile = true;

    std::string dong;
    int soDong = 0;                

    while (std::getline(file, dong)) {
        ++soDong;

        if (soDong == 1 && dong.size() >= 3 &&
            (unsigned char)dong[0] == 0xEF &&
            (unsigned char)dong[1] == 0xBB &&
            (unsigned char)dong[2] == 0xBF) {
            dong.erase(0, 3);
        }

        if (!dong.empty() && dong[dong.size() - 1] == '\r') {
            dong.erase(dong.size() - 1);
        }

        std::string dongSach = trimString(dong);
        if (dongSach.empty() || dongSach[0] == '#') continue;

        ++ketQua.tongSoDong;

        std::vector<std::string> truong = tachTruong(dong);
        if ((int)truong.size() != SO_TRUONG) {
            std::cout << "[CANH BAO] Dong " << soDong << ": can " << SO_TRUONG
                      << " truong, tim thay " << truong.size() << " -> bo qua\n";
            ++ketQua.soSaiDinhDang;
            continue;
        }

        ThiSinh ts;
        ts.maDuThi  = trimString(truong[0]);
        ts.hoTen    = trimString(truong[1]);
        ts.gioiTinh = trimString(truong[2]);
        ts.ngaySinh = trimString(truong[3]);
        ts.queQuan  = trimString(truong[4]);

        if (ts.maDuThi.empty() || ts.hoTen.empty()) {
            std::cout << "[CANH BAO] Dong " << soDong
                      << ": thieu ma du thi hoac ho ten -> bo qua\n";
            ++ketQua.soSaiDinhDang;
            continue;
        }

        if (KIEM_TRA_NGAY_SINH && !ngaySinhHopLe(ts.ngaySinh)) {
            std::cout << "[CANH BAO] Dong " << soDong << ": ngay sinh '" << ts.ngaySinh
                      << "' khong hop le (can dd/mm/yyyy) -> bo qua\n";
            ++ketQua.soSaiDinhDang;
            continue;
        }

        if (insertNode(ht, ts)) {
            ++ketQua.soThanhCong;
        } else {
            std::cout << "[CANH BAO] Dong " << soDong << ": trung ma du thi '"
                      << ts.maDuThi << "' -> bo qua\n";
            ++ketQua.soTrungMa;
        }
    }

    file.close();
    if (kq) *kq = ketQua;
    return true;
}

bool saveToFile(const HashTable& ht, const std::string& filePath) {
    std::ofstream file(filePath.c_str());
    if (!file.is_open()) {
        std::cout << "[LOI] Khong mo duoc file de ghi: " << filePath << "\n";
        return false;
    }

    for (int i = 0; i < NUM_BUCKETS; ++i) {
        const Node* hienTai = ht.buckets[i];
        while (hienTai != nullptr) {
            const ThiSinh& ts = hienTai->data;
            file << lamSachTruocKhiGhi(ts.maDuThi)  << PHAN_CACH
                 << lamSachTruocKhiGhi(ts.hoTen)    << PHAN_CACH
                 << lamSachTruocKhiGhi(ts.gioiTinh) << PHAN_CACH
                 << lamSachTruocKhiGhi(ts.ngaySinh) << PHAN_CACH
                 << lamSachTruocKhiGhi(ts.queQuan)  << "\n";
            hienTai = hienTai->next;
        }
    }

    file.flush();
    bool ghiThanhCong = file.good();
    file.close();

    if (!ghiThanhCong) {
        std::cout << "[LOI] Ghi file bi loi giua chung: " << filePath << "\n";
        return false;
    }
    return true;
}
