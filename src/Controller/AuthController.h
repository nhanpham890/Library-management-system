#ifndef AUTH_CONTROLLER_H
#define AUTH_CONTROLLER_H

#include <QString>
#include "../models/Database.h" // Đường dẫn tới file quản lý CSDL của bạn

class AuthController {
private:
    Database* db; // Con trỏ kết nối cơ sở dữ liệu

public:
    AuthController(Database* database);

    // Xử lý đăng nhập (Trả về true nếu thành công, kèm theo vai trò/role nếu cần)
    bool login(const QString& username, const QString& password, QString& outRole);

    // Xử lý đăng ký tài khoản mới
    bool registerUser(const QString& username, const QString& password, const QString& role);

    // Kiểm tra quyền hạn (Phân quyền)
    bool checkPermission(const QString& username, const QString& requiredRole);
};

#endif // AUTH_CONTROLLER_H