#include "BookController.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

BookController::BookController(Database* database) {
    this->db = database;
}

QList<Book> BookController::searchBooks(const QString& keyword) {
    QList<Book> bookList;
    QSqlQuery query;
    
    // Tìm kiếm gần đúng theo tên sách hoặc tác giả sử dụng câu lệnh LIKE
    query.prepare("SELECT id, title, author, quantity FROM books WHERE title LIKE :kw OR author LIKE :kw");
    query.bindValue(":kw", "%" + keyword + "%");

    if (query.exec()) {
        while (query.next()) {
            int id = query.value(0).toInt();
            QString title = query.value(1).toString();
            QString author = query.value(2).toString();
            int quantity = query.value(3).toInt();

            // Khởi tạo đối tượng Book và thêm vào danh sách kết quả
            Book book(id, title, author, quantity);
            bookList.append(book);
        }
    } else {
        qDebug() << "Lỗi tìm kiếm sách:" << query.lastError().text();
    }

    return bookList;
}

bool BookController::addBook(const QString& title, const QString& author, int quantity) {
    QSqlQuery query;
    query.prepare("INSERT INTO books (title, author, quantity) VALUES (:title, :author, :quantity)");
    query.bindValue(":title", title);
    query.bindValue(":author", author);
    query.bindValue(":quantity", quantity);

    if (!query.exec()) {
        qDebug() << "Lỗi thêm sách:" << query.lastError().text();
        return false;
    }
    return true;
}

bool BookController::deleteBook(int bookId) {
    QSqlQuery query;
    query.prepare("DELETE FROM books WHERE id = :id");
    query.bindValue(":id", bookId);

    if (!query.exec()) {
        qDebug() << "Lỗi xóa sách:" << query.lastError().text();
        return false;
    }
    return true;
}

bool BookController::updateBook(int bookId, const QString& title, const QString& author, int quantity) {
    QSqlQuery query;
    query.prepare("UPDATE books SET title = :title, author = :author, quantity = :quantity WHERE id = :id");
    query.bindValue(":title", title);
    query.bindValue(":author", author);
    query.bindValue(":quantity", quantity);
    query.bindValue(":id", bookId);

    if (!query.exec()) {
        qDebug() << "Lỗi cập nhật sách:" << query.lastError().text();
        return false;
    }
    return true;
}