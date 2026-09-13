#ifndef BOOKLISTVIEW_H
#define BOOKLISTVIEW_H

#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QList>
#include <QFrame>

struct BookData {
    QString title;          // Search by title
    QString author;         // Search by author
    QString subject;        // Search by subject
    QString pubDate;        // Search by publication date
    QString qty;
    QString price;
    QString color;
    QFrame* cardWidget;
};

class BookListView : public QWidget {
    Q_OBJECT
public:
    explicit BookListView(QWidget *parent = nullptr);
    void searchBooks(const QString &criteria, const QString &keyword);

private:
    QGridLayout* gridLayout;
    QList<BookData> allBooks;
};

#endif // BOOKLISTVIEW_H