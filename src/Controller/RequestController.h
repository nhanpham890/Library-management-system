#ifndef REQUESTCONTROLLER_H
#define REQUESTCONTROLLER_H

#include <QString>
#include <QList>
#include <QStringList>

class RequestController {
public:
    static QList<QStringList> getAllRequests();
    static bool approveBorrowBook(const QString &requestId, const QString &condition);
    static bool approveReturnBook(const QString &requestId, const QString &condition, double fineAmount = 0.0);
    static bool approveReserveBook(const QString &requestId);
    static bool approveRenewBook(const QString &requestId);
    static bool checkCondition(const QString &bookId, const QString &condition);
    static bool fineMember(const QString &userId, double amount, const QString &reason);
};

#endif