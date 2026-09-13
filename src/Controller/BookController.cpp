#include "BookController.h"
#include <QSqlQuery>
#include <QVariant>

QList<QStringList> BookController::getAllBooks() {
    QList<QStringList> bookList;
    QSqlQuery query("SELECT id, title, author, category, quantity FROM books ORDER BY id ASC");
    while (query.next()) {
        QStringList bookData;
        for (int col = 0; col < 5; ++col) {
            bookData << query.value(col).toString();
        }
        bookList.append(bookData);
    }
    return bookList;
}

QStringList BookController::getBook(const QString &id) {
    QStringList bookData;
    QSqlQuery query;
    query.prepare("SELECT id, title, author, category, quantity FROM books WHERE id = ?");
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        for (int col = 0; col < 5; ++col) {
            bookData << query.value(col).toString();
        }
    }
    return bookData;
}

bool BookController::addBook(const QString &title, const QString &author, const QString &category, int quantity) {
    QSqlQuery query;
    query.prepare("INSERT INTO books (title, author, category, quantity) VALUES (?, ?, ?, ?)");
    query.addBindValue(title);
    query.addBindValue(author);
    query.addBindValue(category);
    query.addBindValue(quantity);
    return query.exec();
}

bool BookController::editBook(const QString &id, const QString &title, const QString &author, const QString &category, int quantity) {
    QSqlQuery query;
    query.prepare("UPDATE books SET title = ?, author = ?, category = ?, quantity = ? WHERE id = ?");
    query.addBindValue(title);
    query.addBindValue(author);
    query.addBindValue(category);
    query.addBindValue(quantity);
    query.addBindValue(id);
    return query.exec();
}

bool BookController::removeBook(const QString &id) {
    QSqlQuery query;
    query.prepare("DELETE FROM books WHERE id = ?");
    query.addBindValue(id);
    return query.exec();
}