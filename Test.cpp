#include "Test.h"
#include "FileManager.h"
#include "ThiSinhManager.h"
#include "Algorithm.h"
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

ThiSinh taoThiSinh(
    const std::string& ma,
    const std::string& hoTen
) {
    ThiSinh ts;
    ts.maDuThi = ma;
    ts.hoTen = hoTen;
    ts.gioiTinh = "Nam";
    ts.ngaySinh = "01/01/2006";
    ts.queQuan = "Ha Noi";
    return ts;
}

bool kiemTra(
    const std::string& tenTest,
    bool dieuKien
) {
    std::cout << (dieuKien ? "[DAT] " : "[LOI] ")
              << tenTest << '\n';
    return dieuKien;
}

} // namespace

bool taoDuLieuMau(const std::string& tenFile, int soLuong) {
    if (soLuong <= 0) {
        return false;
    }

    std::ofstream file(tenFile.c_str());
    if (!file.is_open()) {
        std::cerr << "Khong mo duoc file: " << tenFile << '\n';
        return false;
    }

    const char* ten[] = {
        "Nguyen Van An",
        "Tran Thi Binh",
        "Le Minh Chau",
        "Pham Quoc Dung",
        "Hoang Thi Em",
        "Vu Gia Huy",
        "Do Khanh Linh",
        "Bui Minh Nam",
        "Dang Thu Phuong",
        "Ngo Tuan Kiet"
    };

    const int soTen = 10;

    for (int i = 1; i <= soLuong; ++i) {
        std::ostringstream ma;
        ma << "TS" << std::setw(6)
           << std::setfill('0') << i;

        file << ma.str() << '|'
             << ten[(i - 1) % soTen] << '|'
             << ((i % 2 == 0) ? "Nu" : "Nam") << '|'
             << "01/01/2006|Ha Noi\n";
    }

    file.close();
    return file.good();
}

bool testBam() {
    std::cout << "\n=== KIEM THU BANG BAM ===\n";

    bool tongKet = true;
    HashTable ht;
    initHashTable(ht);

    tongKet &= kiemTra(
        "Khoi tao bang bam",
        ht.totalCount == 0
    );

    tongKet &= kiemTra(
        "Them thí sinh hop le",
        insertNode(ht, taoThiSinh("TS001", "Nguyen Van An"))
        && ht.totalCount == 1
    );

    tongKet &= kiemTra(
        "Tu choi ma du thi trung",
        !insertNode(ht, taoThiSinh("TS001", "Tran Van Binh"))
    );

    tongKet &= kiemTra(
        "Tu choi ma du thi rong",
        !insertNode(ht, taoThiSinh("   ", "Le Van Cuong"))
    );

    tongKet &= kiemTra(
        "Tu choi ho ten rong",
        !insertNode(ht, taoThiSinh("TS002", "   "))
    );

    tongKet &= kiemTra(
        "Tim thay ma du thi",
        isExistMaDuThi(ht, "TS001")
    );

    tongKet &= kiemTra(
        "Khong tim thay ma du thi",
        !isExistMaDuThi(ht, "TS999")
    );

    tongKet &= kiemTra(
        "Xoa node bang ma du thi",
        removeNodeByID(ht, "TS001")
        && ht.totalCount == 0
    );

    clearHashTable(ht);
    return tongKet;
}

bool testQuanLyThiSinh() {
    std::cout << "\n=== KIEM THU QUAN LY THI SINH ===\n";

    bool tongKet = true;
    HashTable ht;
    initHashTable(ht);

    tongKet &= kiemTra(
        "Them thi sinh TS010",
        themThiSinh(ht, taoThiSinh("TS010", "Nguyen Van An"))
    );

    tongKet &= kiemTra(
        "Them thi sinh TS011",
        themThiSinh(ht, taoThiSinh("TS011", "Le Van An"))
    );

    tongKet &= kiemTra(
        "Tu choi trung ma du thi",
        !themThiSinh(ht, taoThiSinh("TS010", "Tran Thi Binh"))
    );

    Node* timTheoID =
        timTheoMa(ht, " ts010 ");

    tongKet &= kiemTra(
        "Tim theo ma khong phan biet hoa thuong",
        timTheoID != nullptr
        && timTheoID->data.maDuThi == "TS010"
    );

    tongKet &= kiemTra(
        "Tim ma khong ton tai",
        timTheoMa(ht, "TS999") == nullptr
    );

    tongKet &= kiemTra(
        "Tim theo ten An",
        timTheoTen(ht, "An") == 2
    );

    tongKet &= kiemTra(
        "Tim ten rong",
        timTheoTen(ht, "   ") == 0
    );

    tongKet &= kiemTra(
        "Tong so du lieu la 2",
        ht.totalCount == 2
    );

    clearHashTable(ht);
    return tongKet;
}

bool testAlgorithm() {
    std::cout << "\n=== KIEM THU THUAT TOAN ===\n";

    bool tongKet = true;

    // Test sắp xếp theo toàn bộ họ tên.
    std::vector<ThiSinh> ds;
    ds.push_back(taoThiSinh("TS01", "Trần Thị Ánh"));
    ds.push_back(taoThiSinh("TS02", "Nguyễn Văn Bình"));
    ds.push_back(taoThiSinh("TS03", "Lê Văn An"));
    ds.push_back(taoThiSinh("TS04", "Doãn Minh"));
    ds.push_back(taoThiSinh("TS05", "Đặng Văn Dũng"));

    sapXepTheoTen(
        ds,
        [](const ThiSinh& ts) {
            return ts.hoTen;
        }
    );

    bool sapXepDung =
        ds[0].maDuThi == "TS04"
        && ds[1].maDuThi == "TS05"
        && ds[2].maDuThi == "TS02"
        && ds[3].maDuThi == "TS01"
        && ds[4].maDuThi == "TS03";

    tongKet &= kiemTra(
        "Sap xep ten tieng Viet",
        sapXepDung
    );

    // Stable sort voi hai ten co khoa giong nhau.
    std::vector<ThiSinh> dsStable;
    dsStable.push_back(taoThiSinh("TS01", "An"));
    dsStable.push_back(taoThiSinh("TS02", "Án"));

    sapXepTheoTen(
        dsStable,
        [](const ThiSinh& ts) {
            return ts.hoTen;
        }
    );

    tongKet &= kiemTra(
        "Stable sort voi ten co va khong dau",
        dsStable[0].maDuThi == "TS01"
        && dsStable[1].maDuThi == "TS02"
    );

    // Chia 10 thí sinh vào 3 phòng: 4 - 3 - 3.
    ds.clear();
    for (int i = 0; i < 10; ++i) {
        std::ostringstream ma;
        ma << "TS" << i;
        ds.push_back(taoThiSinh(ma.str(), "Thi Sinh"));
    }

    std::vector<std::vector<ThiSinh> > cacPhong;
    bool chiaThanhCong =
        chiaThiSinhVaoPhong(ds, 3, cacPhong);

    tongKet &= kiemTra(
        "Chia 10 thi sinh thanh 3 phong 4-3-3",
        chiaThanhCong
        && cacPhong.size() == 3
        && cacPhong[0].size() == 4
        && cacPhong[1].size() == 3
        && cacPhong[2].size() == 3
    );

    // Kiểm tra thứ tự sau khi chia phòng.
    if (chiaThanhCong) {
        std::vector<std::string> thuTuPhong;

        for (std::size_t i = 0; i < cacPhong.size(); ++i) {
            for (std::size_t j = 0;
                 j < cacPhong[i].size();
                 ++j) {
                thuTuPhong.push_back(cacPhong[i][j].maDuThi);
            }
        }

        bool baoToanThuTu = true;
        for (std::size_t i = 0; i < ds.size(); ++i) {
            if (thuTuPhong[i] != ds[i].maDuThi) {
                baoToanThuTu = false;
                break;
            }
        }

        tongKet &= kiemTra(
            "Bao toan thu tu thi sinh",
            baoToanThuTu
        );
    }

    // So phòng không hợp lệ.
    std::vector<std::vector<ThiSinh> > phongCu(1);
    phongCu[0].push_back(taoThiSinh("OLD", "Du lieu cu"));

    bool chia0Phong =
        chiaThiSinhVaoPhong(ds, 0, phongCu);

    tongKet &= kiemTra(
        "Tu choi 0 phong",
        !chia0Phong && phongCu.empty()
    );

    // Nhiều phòng hơn số thí sinh.
    std::vector<ThiSinh> motNguoi;
    motNguoi.push_back(taoThiSinh("TS01", "Thi Sinh"));

    std::vector<std::vector<ThiSinh> > nhieuPhong;
    bool chiaNhieuPhong =
        chiaThiSinhVaoPhong(motNguoi, 3, nhieuPhong);

    tongKet &= kiemTra(
        "Nhieu phong hon so thi sinh",
        chiaNhieuPhong
        && nhieuPhong.size() == 3
        && nhieuPhong[0].size() == 1
        && nhieuPhong[1].empty()
        && nhieuPhong[2].empty()
    );

    return tongKet;
}

bool testFileManager() {
    std::cout << "\n=== KIEM THU QUAN LY FILE ===\n";

    bool tongKet = true;
    const std::string fileVao = "test_input.tmp";
    const std::string fileRa = "test_output.tmp";

    std::ofstream file(fileVao.c_str());
    file << "\xEF\xBB\xBF"
         << "TS001|Nguyen Van An|Nam|01/01/2006|Ha Noi\n";
    file << "TS002|Tran Thi Binh|Nu|29/02/2004|Da Nang\n";
    file << "TS001|Ten Trung Ma|Nam|02/02/2006|Hue\n";
    file << "DONG_SAI_DINH_DANG\n";
    file << "|Thieu Ma|Nam|03/03/2006|Can Tho\n";
    file << "TS003|Thieu Ngay Sinh|Nam|31/02/2006|Ha Noi\n";
    file.close();

    HashTable ht;
    initHashTable(ht);

    LoadResult ketQua;
    bool docThanhCong =
        loadFromFile(ht, fileVao, &ketQua);

    tongKet &= kiemTra(
        "Mo va doc file thanh cong",
        docThanhCong && ketQua.moDuocFile
    );

    tongKet &= kiemTra(
        "Tai dung 2 thi sinh hop le",
        ht.totalCount == 2
        && ketQua.soThanhCong == 2
    );

    tongKet &= kiemTra(
        "Bo qua dong trung ma",
        ketQua.soTrungMa == 1
    );

    tongKet &= kiemTra(
        "Bo qua dong sai dinh dang",
        ketQua.soSaiDinhDang >= 3
    );

    tongKet &= kiemTra(
        "Ghi du lieu ra file",
        saveToFile(ht, fileRa)
    );

    HashTable htDocLai;
    initHashTable(htDocLai);

    bool docLai =
        loadFromFile(htDocLai, fileRa);

    tongKet &= kiemTra(
        "Doc lai du lieu da ghi",
        docLai
        && htDocLai.totalCount == 2
        && isExistMaDuThi(htDocLai, "TS001")
        && isExistMaDuThi(htDocLai, "TS002")
    );

    clearHashTable(ht);
    clearHashTable(htDocLai);

    std::remove(fileVao.c_str());
    std::remove(fileRa.c_str());

    return tongKet;
}

bool testTai10K() {
    std::cout << "\n=== KIEM THU TAI 10.000 THI SINH ===\n";

    const std::string fileMau = "test_mock_10k.tmp";

    bool taoThanhCong =
        taoDuLieuMau(fileMau, 10000);

    HashTable ht;
    initHashTable(ht);

    LoadResult ketQua;
    bool napThanhCong =
        taoThanhCong
        && loadFromFile(ht, fileMau, &ketQua);

    bool dungSoLuong =
        napThanhCong
        && ht.totalCount == 10000
        && ketQua.soThanhCong == 10000;

    kiemTra(
        "Nap du lieu 10.000 thi sinh",
        dungSoLuong
    );

    clearHashTable(ht);
    std::remove(fileMau.c_str());

    return dungSoLuong;
}

bool chayTatCaTest() {
    std::cout << "========== BAT DAU KIEM THU ==========\n";

    bool ketQua = true;

    ketQua &= testBam();
    ketQua &= testQuanLyThiSinh();
    ketQua &= testAlgorithm();
    ketQua &= testFileManager();
    ketQua &= testTai10K();

    std::cout << "\n========== "
              << (ketQua ? "TAT CA TEST DEU DAT"
                         : "CO TEST BI LOI")
              << " ==========\n";

    return ketQua;
}

#ifdef CHAY_TEST_RIENg
int main() {
    return chayTatCaTest() ? 0 : 1;
}
#endif