#pragma once
#include <QAbstractTableModel>
#include <vector>
#include "book.h"

class BookTableModel : public QAbstractTableModel {
    Q_OBJECT
private:
    std::vector<Book> books;
    QStringList headers = { "Mã sách", "Tên sách", "Tác giả", "Thể loại", "Số lượng" };

public:
    BookTableModel(QObject* parent = nullptr) : QAbstractTableModel(parent) {}

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        if (parent.isValid()) return 0;
        return static_cast<int>(books.size());
    }

    int columnCount(const QModelIndex& parent = QModelIndex()) const override {
        if (parent.isValid()) return 0;
        return headers.size();
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() >= static_cast<int>(books.size()))
            return QVariant();

        const auto& book = books[index.row()];

        if (role == Qt::DisplayRole) {
            switch (index.column()) {
            case 0: return book.getId();
            case 1: return book.getTitle();
            case 2: return book.getAuthor();
            case 3: return book.getCategory();
            case 4: return book.getQuantity();
            default: return QVariant();
            }
        }
        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        if (role != Qt::DisplayRole) return QVariant();

        if (orientation == Qt::Horizontal && section < headers.size()) {
            return headers[section];
        }
        return QVariant();
    }

    void addBook(const Book& book) {
        beginInsertRows(QModelIndex(), books.size(), books.size());
        books.push_back(book);
        endInsertRows();
    }
};