#pragma once
#include <QString>

class Member {
private:
    QString memberId;
    QString name;
    QString email;

public:
    Member() = default;
    Member(QString id, QString name, QString email)
        : memberId(id), name(name), email(email) {
    }

    QString getMemberId() const { return memberId; }
    QString getName() const { return name; }
    QString getEmail() const { return email; }

    void setName(const QString& n) { name = n; }
    void setEmail(const QString& e) { email = e; }
};