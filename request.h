#pragma once
#include <QString>
#include <QDate>

class Request {
private:
    QString requestId;
    QString userId;
    QString bookId;
    QDate borrowDate;
    QDate dueDate;
    QString status; // "Pending", "Borrowed", "Returned", "Overdue"

public:
    Request() = default;
    Request(QString reqId, QString uId, QString bId, QDate bDate, QDate dDate, QString stat)
        : requestId(reqId), userId(uId), bookId(bId), borrowDate(bDate), dueDate(dDate), status(stat) {
    }

    QString getRequestId() const { return requestId; }
    QString getUserId() const { return userId; }
    QString getBookId() const { return bookId; }
    QDate getBorrowDate() const { return borrowDate; }
    QDate getDueDate() const { return dueDate; }
    QString getStatus() const { return status; }

    void setStatus(const QString& stat) { status = stat; }
};
