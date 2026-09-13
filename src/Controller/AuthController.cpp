#include "AuthController.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCryptographicHash> // Dùng để mã hóa mật khẩu nếu cần (khuyên dùng SHA-256)

AuthController::AuthController(Database* database) {
    this->db = database;
}

bool AuthController::login(const QString& username, const QString& password, QString& outRole) {
    // Ví dụ truy vấn kiểm tra thông tin tài khoản trong CSDL
    // Lưu ý: Trong thực tế nên mã hóa password trước khi so sánh với database
    QSqlQuery query;
    query.prepare("SELECT password, role FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        QString storedPassword = query.value(0).toString();
        QString role = query.value(1).toString();

        // So sánh mật khẩu (Nên dùng băm mật khẩu như bcrypt hoặc SHA256)
        if (storedPassword == password) { 
            outRole = role; // Trả về phân quyền (ví dụ: Admin, Member)
            return true;
        }
    }
    return false;
}

bool AuthController::registerUser(const QString& username, const QString& password, const QString& role) {
    // Kiểm tra xem tài khoản đã tồn tại chưa hoặc tiến hành chèn vào CSDL
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, role) VALUES (:username, :password, :role)");
    query.bindValue(":username", username);
    query.bindValue(":password", password); // Nhớ mã hóa mật khẩu trước khi lưu
    query.bindValue(":role", role);

    if (!query.exec()) {
        qDebug() << "Lỗi đăng ký:" << query.lastError().text();
        return false;
    }
    return true;
}

bool AuthController::checkPermission(const QString& username, const QString& requiredRole) {
    QSqlQuery query;
    query.prepare("SELECT role FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        QString role = query.value(0).toString();
        return (role == requiredRole);
    }
    return false;
}