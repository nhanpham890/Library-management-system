#pragma once
#include <QString>

class User {
private:
    QString userId;
    QString username;
    QString password;
    QString role; // "Admin", "Librarian", "Member"

public:
    User() = default;
    User(QString id, QString uname, QString pwd, QString r)
        : userId(id), username(uname), password(pwd), role(r) {
    }

    QString getUserId() const { return userId; }
    QString getUsername() const { return username; }
    QString getPassword() const { return password; }
    QString getRole() const { return role; }

    void setPassword(const QString& pwd) { password = pwd; }
    void setRole(const QString& r) { role = r; }
};
