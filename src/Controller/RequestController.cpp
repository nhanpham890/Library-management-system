#include "RequestController.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDate>

QList<QStringList> RequestController::getAllRequests() {
    QList<QStringList> requestList;
    QSqlQuery query("SELECT id, user_id, book_id, type, request_date, status FROM requests ORDER BY id ASC");
    while (query.next()) {
        QStringList reqData;
        for (int col = 0; col < 6; ++col) {
            reqData << query.value(col).toString();
        }
        requestList.append(reqData);
    }
    return requestList;
}

bool RequestController::checkCondition(const QString &bookId, const QString &condition) {
    QSqlQuery query;
    query.prepare("UPDATE books SET category = category WHERE id = ?");
    query.addBindValue(bookId);
    return query.exec();
}

bool RequestController::fineMember(const QString &userId, double amount, const QString &reason) {
    QSqlQuery createFineTable;
    createFineTable.exec("CREATE TABLE IF NOT EXISTS fines ("
                         "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "user_id INTEGER, "
                         "amount REAL, "
                         "reason TEXT, "
                         "fine_date DATE)");

    QSqlQuery query;
    query.prepare("INSERT INTO fines (user_id, amount, reason, fine_date) VALUES (?, ?, ?, ?)");
    query.addBindValue(userId);
    query.addBindValue(amount);
    query.addBindValue(reason);
    query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));
    return query.exec();
}

bool RequestController::approveBorrowBook(const QString &requestId, const QString &condition) {
    QSqlQuery query;
    query.prepare("UPDATE requests SET status = 'Approved' WHERE id = ?");
    query.addBindValue(requestId);
    return query.exec();
}

bool RequestController::approveReturnBook(const QString &requestId, const QString &condition, double fineAmount) {
    QSqlQuery query;
    query.prepare("SELECT user_id, book_id FROM requests WHERE id = ?");
    query.addBindValue(requestId);
    
    if (query.exec() && query.next()) {
        QString userId = query.value(0).toString();
        QString bookId = query.value(1).toString();

        checkCondition(bookId, condition);

        if (fineAmount > 0.0) {
            fineMember(userId, fineAmount, "Condition violation: " + condition);
        }

        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE requests SET status = 'Returned' WHERE id = ?");
        updateQuery.addBindValue(requestId);
        return updateQuery.exec();
    }
    return false;
}

bool RequestController::approveReserveBook(const QString &requestId) {
    QSqlQuery query;
    query.prepare("UPDATE requests SET status = 'Reserved' WHERE id = ?");
    query.addBindValue(requestId);
    return query.exec();
}

bool RequestController::approveRenewBook(const QString &requestId) {
    QSqlQuery query;
    query.prepare("UPDATE requests SET status = 'Renewed' WHERE id = ?");
    query.addBindValue(requestId);
    return query.exec();
}