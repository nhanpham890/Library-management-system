#ifndef ADMINCONTROLLER_H
#define ADMINCONTROLLER_H

#include <QString>
#include <QList>
#include <QStringList>

class AdminController {
public:
    static QList<QStringList> getAllMembers();
    static QStringList viewMember(const QString &id);
    static bool addMember(const QString &username, const QString &password, const QString &email, const QString &phone, const QString &role);
    static bool deleteMember(const QString &id);
    static bool suspendMember(const QString &id);
};

#endif