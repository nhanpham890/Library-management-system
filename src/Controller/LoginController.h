#ifndef LOGINCONTROLLER_H
#define LOGINCONTROLLER_H

#include <QString>

class LoginController {
public:
    static bool login(const QString &username, const QString &password, QString &outRole);
    static bool registerUser(const QString &username, const QString &password, const QString &email, const QString &phone);
};

#endif