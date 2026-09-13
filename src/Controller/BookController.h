#ifndef BOOKCONTROLLER_H
#define BOOKCONTROLLER_H

#include <QString>
#include <QList>
#include <QStringList>

class BookController {
public:
    static QList<QStringList> getAllBooks();
    static QStringList getBook(const QString &id);
    static bool addBook(const QString &title, const QString &author, const QString &category, int quantity);
    static bool editBook(const QString &id, const QString &title, const QString &author, const QString &category, int quantity);
    static bool removeBook(const QString &id);
};

#endif