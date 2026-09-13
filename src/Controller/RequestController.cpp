#include "RequestController.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

RequestController::RequestController(Database* database) {
    this->db = database;
}

bool RequestController::createBorrowRequest(int userId, int bookId) {
    QSqlQuery query;
    // Trạng thái mặc định ban đầu có thể là "Pending" (Đang chờ duyệt)
    query.prepare("INSERT INTO requests (user_id, book_id, status, request_date) VALUES (:user_id, :book_id, 'Pending', NOW())");
    query.bindValue(":user_id", userId);
    query.bindValue(":book_id", bookId);

    if (!query.exec()) {
        qDebug() << "Lỗi tạo yêu cầu mượn sách:" << query.lastError().text();
        return false;
    }
    return true;
}

bool RequestController::approveRequest(int requestId) {
    QSqlQuery query;
    // Khi Admin duyệt, đổi trạng thái thành "Approved" và có thể trừ số lượng sách trong kho
    query.prepare("UPDATE requests SET status = 'Approved' WHERE id = :id");
    query.bindValue(":id", requestId);

    if (!query.exec()) {
        qDebug() << "Lỗi duyệt yêu cầu:" << query.lastError().text();
        return false;
    }
    return true;
}

bool RequestController::returnBook(int requestId, int bookId) {
    // Sử dụng transaction để đảm bảo cập nhật trạng thái phiếu mượn và cộng lại số lượng sách thành công cùng lúc
    QSqlDatabase::database().transaction();

    QSqlQuery queryReq;
    queryReq.prepare("UPDATE requests SET status = 'Returned', return_date = NOW() WHERE id = :id");
    queryReq.bindValue(":id", requestId);

    QSqlQuery queryBook;
    queryBook.prepare("UPDATE books SET quantity = quantity + 1 WHERE id = :book_id");
    queryBook.bindValue(":book_id", bookId);

    if (queryReq.exec() && queryBook.exec()) {
        QSqlDatabase::database().commit();
        return true;
    } else {
        QSqlDatabase::database().rollback();
        qDebug() << "Lỗi trả sách:" << queryReq.lastError().text();
        return false;
    }
}

bool RequestController::extendRequest(int requestId) {
    QSqlQuery query;
    // Cập nhật lại ngày hết hạn mượn (ví dụ cộng thêm thời gian)
    query.prepare("UPDATE requests SET due_date = DATE_ADD(due_date, INTERVAL 7 DAY) WHERE id = :id");
    query.bindValue(":id", requestId);

    if (!query.exec()) {
        qDebug() << "Lỗi gia hạn sách:" << query.lastError().text();
        return false;
    }
    return true;
}

QList<Request> RequestController::getAllRequests() {
    QList<Request> requestList;
    QSqlQuery query("SELECT id, user_id, book_id, status, request_date, due_date FROM requests");

    while (query.next()) {
        int id = query.value(0).toInt();
        int userId = query.value(1).toInt();
        int bookId = query.value(2).toInt();
        QString status = query.value(3).toString();
        QString requestDate = query.value(4).toString();
        QString dueDate = query.value(5).toString();

        Request req(id, userId, bookId, status, requestDate, dueDate);
        requestList.append(req);
    }

    return requestList;
}
