#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <functional>
#include <string>
#include <vector>

#include "HashTable.h"

// Sắp xếp danh sách theo họ tên tăng dần.
// layHoTen dùng để lấy chuỗi họ tên từ struct ThiSinh.
void sapXepTheoTen(
    std::vector<ThiSinh>& danhSach,
    const std::function<std::string(const ThiSinh&)>& layHoTen
);

// Chia danh sách vào soPhong phòng theo thứ tự.
// Trả về false nếu soPhong <= 0; khi đó ketQuaPhong được xóa.
// Thành công thì ketQuaPhong có đúng soPhong phần tử.
bool chiaThiSinhVaoPhong(
    const std::vector<ThiSinh>& danhSach,
    int soPhong,
    std::vector<std::vector<ThiSinh>>& ketQuaPhong
);

#endif