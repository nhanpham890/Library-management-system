#include "LoginController.h"
#include <QSqlQuery>
#include <QVariant>

bool LoginController::login(const QString &username, const QString &password, QString &outRole) {
    QSqlQuery query;
    query.prepare("SELECT password, role FROM users WHERE username = ?");
    query.addBindValue(username);
    if (query.exec() && query.next()) {
        if (query.value(0).toString() == password) {
            outRole = query.value(1).toString();
            return true;
        }
    }
    return false;
}

bool LoginController::registerUser(const QString &username, const QString &password, const QString &email, const QString &phone) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, email, phone) VALUES (?, ?, ?, ?)");
    query.addBindValue(username);
    query.addBindValue(password);
    query.addBindValue(email);
    query.addBindValue(phone);
    return query.exec();
}