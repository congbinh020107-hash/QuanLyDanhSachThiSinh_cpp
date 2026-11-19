#include "Algorithm.h"
#include <algorithm>
#include <cstdint>

namespace {

// Đọc một ký tự UTF-8 và trả về mã Unicode cùng số byte đã đọc.
// Nếu dữ liệu không hợp lệ, coi byte hiện tại là một ký tự riêng.
std::uint32_t docMaUnicode(
    const std::string& chuoi,
    std::size_t viTri,
    std::size_t& soByte
) {
    const unsigned char c0 =
        static_cast<unsigned char>(chuoi[viTri]);

    if (c0 < 0x80) {
        soByte = 1;
        return c0;
    }

    if ((c0 & 0xE0) == 0xC0 && viTri + 1 < chuoi.size()) {
        const unsigned char c1 =
            static_cast<unsigned char>(chuoi[viTri + 1]);

        if ((c1 & 0xC0) == 0x80) {
            soByte = 2;
            return ((c0 & 0x1F) << 6) | (c1 & 0x3F);
        }
    }

    if ((c0 & 0xF0) == 0xE0 && viTri + 2 < chuoi.size()) {
        const unsigned char c1 =
            static_cast<unsigned char>(chuoi[viTri + 1]);
        const unsigned char c2 =
            static_cast<unsigned char>(chuoi[viTri + 2]);

        if ((c1 & 0xC0) == 0x80 && (c2 & 0xC0) == 0x80) {
            soByte = 3;
            return ((c0 & 0x0F) << 12)
                 | ((c1 & 0x3F) << 6)
                 | (c2 & 0x3F);
        }
    }

    if ((c0 & 0xF8) == 0xF0 && viTri + 3 < chuoi.size()) {
        const unsigned char c1 =
            static_cast<unsigned char>(chuoi[viTri + 1]);
        const unsigned char c2 =
            static_cast<unsigned char>(chuoi[viTri + 2]);
        const unsigned char c3 =
            static_cast<unsigned char>(chuoi[viTri + 3]);

        if ((c1 & 0xC0) == 0x80
            && (c2 & 0xC0) == 0x80
            && (c3 & 0xC0) == 0x80) {
            soByte = 4;
            return ((c0 & 0x07) << 18)
                 | ((c1 & 0x3F) << 12)
                 | ((c2 & 0x3F) << 6)
                 | (c3 & 0x3F);
        }
    }

    soByte = 1;
    return c0;
}

// Đưa ký tự tiếng Việt về chữ cái Latin không dấu để tạo khóa sắp xếp.
char chuCaiKhongDau(std::uint32_t cp) {
    switch (cp) {
        // A, Ă, Â
        case 0x00E0: case 0x00E1: case 0x1EA3: case 0x00E3:
        case 0x1EA1: case 0x0103: case 0x1EB1: case 0x1EAF:
        case 0x1EB3: case 0x1EB5: case 0x1EB7: case 0x00E2:
        case 0x1EA7: case 0x1EA5: case 0x1EA9: case 0x1EAB:
        case 0x1EAD:
        case 0x00C0: case 0x00C1: case 0x1EA2: case 0x00C3:
        case 0x1EA0: case 0x0102: case 0x1EB0: case 0x1EAE:
        case 0x1EB2: case 0x1EB4: case 0x1EB6: case 0x00C2:
        case 0x1EA6: case 0x1EA4: case 0x1EA8: case 0x1EAA:
        case 0x1EAC:
            return 'a';

        // E, Ê
        case 0x00E8: case 0x00E9: case 0x1EBB: case 0x1EBD:
        case 0x1EB9: case 0x00EA: case 0x1EC1: case 0x1EBF:
        case 0x1EC3: case 0x1EC5: case 0x1EC7:
        case 0x00C8: case 0x00C9: case 0x1EBA: case 0x1EBC:
        case 0x1EB8: case 0x00CA: case 0x1EC0: case 0x1EBE:
        case 0x1EC2: case 0x1EC4: case 0x1EC6:
            return 'e';

        // I
        case 0x00EC: case 0x00ED: case 0x1EC9: case 0x0129:
        case 0x1ECB: case 0x00CC: case 0x00CD: case 0x1EC8:
        case 0x0128: case 0x1ECA:
            return 'i';

        // O, Ô, Ơ
        case 0x00F2: case 0x00F3: case 0x1ECF: case 0x00F5:
        case 0x1ECD: case 0x00F4: case 0x1ED3: case 0x1ED1:
        case 0x1ED5: case 0x1ED7: case 0x1ED9: case 0x01A1:
        case 0x1EDD: case 0x1EDB: case 0x1EDF: case 0x1EE1:
        case 0x1EE3:
        case 0x00D2: case 0x00D3: case 0x1ECE: case 0x00D5:
        case 0x1ECC: case 0x00D4: case 0x1ED2: case 0x1ED0:
        case 0x1ED4: case 0x1ED6: case 0x1ED8: case 0x01A0:
        case 0x1EDC: case 0x1EDA: case 0x1EDE: case 0x1EE0:
        case 0x1EE2:
            return 'o';

        // U, Ư
        case 0x00F9: case 0x00FA: case 0x1EE7: case 0x0169:
        case 0x1EE5: case 0x01B0: case 0x1EEB: case 0x1EE9:
        case 0x1EED: case 0x1EEF: case 0x1EF1:
        case 0x00D9: case 0x00DA: case 0x1EE6: case 0x0168:
        case 0x1EE4: case 0x01AF: case 0x1EEA: case 0x1EE8:
        case 0x1EEC: case 0x1EEE: case 0x1EF0:
            return 'u';

        // Y
        case 0x1EF3: case 0x00FD: case 0x1EF7: case 0x1EF9:
        case 0x1EF5: case 0x1EF2: case 0x00DD: case 0x1EF6:
        case 0x1EF8: case 0x1EF4:
            return 'y';

        // Đ, đ
        case 0x0110: case 0x0111:
            return 'd';

        default:
            if (cp >= 'A' && cp <= 'Z') {
                return static_cast<char>(cp - 'A' + 'a');
            }
            if (cp < 0x80) {
                return static_cast<char>(cp);
            }
            return '\0';
    }
}

std::string taoKhoaTen(const std::string& ten) {
    std::string khoa;

    for (std::size_t i = 0; i < ten.size();) {
        std::size_t soByte = 1;
        const std::uint32_t cp = docMaUnicode(ten, i, soByte);
        const char kyTu = chuCaiKhongDau(cp);

        if (kyTu != '\0') {
            khoa.push_back(kyTu);
        } else {
            // Giữ nguyên ký tự Unicode không thuộc bảng chữ cái tiếng Việt.
            khoa.append(ten, i, soByte);
        }

        i += soByte;
    }

    return khoa;
}

} // namespace

void sapXepTheoTen(
    std::vector<ThiSinh>& danhSach,
    const std::function<std::string(const ThiSinh&)>& layHoTen
) {
    if (!layHoTen) {
        return;
    }

    std::stable_sort(
        danhSach.begin(),
        danhSach.end(),
        [&layHoTen](const ThiSinh& a, const ThiSinh& b) {
            return taoKhoaTen(layHoTen(a)) < taoKhoaTen(layHoTen(b));
        }
    );
}

bool chiaThiSinhVaoPhong(
    const std::vector<ThiSinh>& danhSach,
    int soPhong,
    std::vector<std::vector<ThiSinh>>& ketQuaPhong
) {
    ketQuaPhong.clear();

    if (soPhong <= 0) {
        return false;
    }

    ketQuaPhong.resize(static_cast<std::size_t>(soPhong));

    const std::size_t tongThiSinh = danhSach.size();
    const std::size_t tongPhong = static_cast<std::size_t>(soPhong);
    const std::size_t moiPhong = tongThiSinh / tongPhong;
    const std::size_t soPhongDu = tongThiSinh % tongPhong;

    std::size_t viTri = 0;

    for (std::size_t phong = 0; phong < tongPhong; ++phong) {
        const std::size_t soThiSinhPhong =
            moiPhong + (phong < soPhongDu ? 1 : 0);

        for (std::size_t i = 0; i < soThiSinhPhong; ++i) {
            ketQuaPhong[phong].push_back(danhSach[viTri]);
            ++viTri;
        }
    }

    return true;
}