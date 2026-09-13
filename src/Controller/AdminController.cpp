#include "AdminController.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AdminController::AdminController(Database* database) {
    this->db = database;
}

QList<User> AdminController::getAllUsers() {
    QList<User> userList;
    QSqlQuery query("SELECT id, username, role, status FROM users");

    while (query.next()) {
        int id = query.value(0).toInt();
        QString username = query.value(1).toString();
        QString role = query.value(2).toString();
        bool status = query.value(3).toBool(); // true: hoạt động, false: bị khóa

        User user(id, username, role, status);
        userList.append(user);
    }
    return userList;
}

bool AdminController::toggleUserStatus(int userId, bool isActive) {
    QSqlQuery query;
    query.prepare("UPDATE users SET status = :status WHERE id = :id");
    query.bindValue(":status", isActive);
    query.bindValue(":id", userId);

    if (!query.exec()) {
        qDebug() << "Lỗi cập nhật trạng thái tài khoản:" << query.lastError().text();
        return false;
    }
    return true;
}

bool AdminController::deleteUser(int userId) {
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE id = :id");
    query.bindValue(":id", userId);

    if (!query.exec()) {
        qDebug() << "Lỗi xóa tài khoản:" << query.lastError().text();
        return false;
    }
    return true;
}

int AdminController::getSystemStat(const QString& statType) {
    QSqlQuery query;
    
    if (statType == "books") {
        query.exec("SELECT COUNT(*) FROM books");
    } else if (statType == "users") {
        query.exec("SELECT COUNT(*) FROM users");
    } else if (statType == "pending_requests") {
        query.exec("SELECT COUNT(*) FROM requests WHERE status = 'Pending'");
    }

    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}