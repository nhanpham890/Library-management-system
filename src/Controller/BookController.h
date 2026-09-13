#ifndef BOOK_CONTROLLER_H
#define BOOK_CONTROLLER_H

#include <QString>
#include <QList>
#include "../models/Database.h"
#include "../models/Book.h" // Class thực thể sách (Book)

class BookController {
private:
    Database* db; // Con trỏ kết nối cơ sở dữ liệu

public:
    BookController(Database* database);

    // Tìm kiếm sách theo từ khóa (tên sách hoặc tác giả)
    QList<Book> searchBooks(const QString& keyword);

    // Thêm sách mới vào thư viện (Dành cho Admin)
    bool addBook(const QString& title, const QString& author, int quantity);

    // Xóa sách khỏi hệ thống theo ID
    bool deleteBook(int bookId);

    // Cập nhật thông tin sách
    bool updateBook(int bookId, const QString& title, const QString& author, int quantity);
};

#endif // BOOK_CONTROLLER_H
