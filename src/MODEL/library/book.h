#pragma once
#include <QString>

class Book {
private:
    QString id;
    QString title;
    QString author;
    QString category;
    int quantity;

public:
    Book() = default;
    Book(QString id, QString title, QString author, QString category, int quantity)
        : id(id), title(title), author(author), category(category), quantity(quantity) {
    }

    QString getId() const { return id; }
    QString getTitle() const { return title; }
    QString getAuthor() const { return author; }
    QString getCategory() const { return category; }
    int getQuantity() const { return quantity; }

    void setTitle(const QString& t) { title = t; }
    void setAuthor(const QString& a) { author = a; }
    void setCategory(const QString& c) { category = c; }
    void setQuantity(int q) { quantity = q; }
};