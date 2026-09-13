#ifndef ADMIN_CONTROLLER_H
#define ADMIN_CONTROLLER_H

#include <QString>
#include <QList>
#include "../models/Database.h"
#include "../models/User.h" // Class thực thể người dùng

class AdminController {
private:
    Database* db; // Con trỏ kết nối cơ sở dữ liệu

public:
    AdminController(Database* database);

    // Lấy danh sách toàn bộ người dùng trong hệ thống
    QList<User> getAllUsers();

    // Khóa hoặc mở khóa tài khoản người dùng
    bool toggleUserStatus(int userId, bool isActive);

    // Xóa tài khoản người dùng khỏi hệ thống
    bool deleteUser(int userId);

    // Lấy thống kê hệ thống (số lượng sách, số lượng user, số yêu cầu đang chờ duyệt...)
    int getSystemStat(const QString& statType);
};

#endif // ADMIN_CONTROLLER_H