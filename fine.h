#pragma once
#include <QString>

class Fine {
private:
    QString fineId;
    QString requestId;
    double amount;
    QString status; // "Unpaid", "Paid"

public:
    Fine() = default;
    Fine(QString fId, QString reqId, double amt, QString stat)
        : fineId(fId), requestId(reqId), amount(amt), status(stat) {
    }

    QString getFineId() const { return fineId; }
    QString getRequestId() const { return requestId; }
    double getAmount() const { return amount; }
    QString getStatus() const { return status; }

    void setStatus(const QString& stat) { status = stat; }
};
