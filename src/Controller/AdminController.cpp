#include "AdminController.h"
#include <QSqlQuery>
#include <QVariant>

QList<QStringList> AdminController::getAllMembers() {
    QList<QStringList> userList;
    QSqlQuery query("SELECT id, username, email, phone, role FROM users ORDER BY id ASC");
    while (query.next()) {
        QStringList userData;
        for (int col = 0; col < 5; ++col) {
            userData << query.value(col).toString();
        }
        userList.append(userData);
    }
    return userList;
}

QStringList AdminController::viewMember(const QString &id) {
    QStringList memberData;
    QSqlQuery query;
    query.prepare("SELECT id, username, email, phone, role FROM users WHERE id = ?");
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        for (int col = 0; col < 5; ++col) {
            memberData << query.value(col).toString();
        }
    }
    return memberData;
}

bool AdminController::addMember(const QString &username, const QString &password, const QString &email, const QString &phone, const QString &role) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, email, phone, role) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(username);
    query.addBindValue(password);
    query.addBindValue(email);
    query.addBindValue(phone);
    query.addBindValue(role.isEmpty() ? "Member" : role);
    return query.exec();
}

bool AdminController::deleteMember(const QString &id) {
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE id = ?");
    query.addBindValue(id);
    return query.exec();
}

bool AdminController::suspendMember(const QString &id) {
    QSqlQuery query;
    query.prepare("UPDATE users SET role = 'Suspended' WHERE id = ?");
    query.addBindValue(id);
    return query.exec();
}