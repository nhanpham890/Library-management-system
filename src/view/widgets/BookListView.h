#ifndef BOOKLISTVIEW_H
#define BOOKLISTVIEW_H

#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QString>

class BookListView : public QWidget {
    Q_OBJECT
public:
    explicit BookListView(QWidget *parent = nullptr);
    void loadBooksFromDatabase(const QString &keyword = "", const QString &searchType = "Search by title");

private:
    QScrollArea *scrollArea;
    QWidget *containerWidget;
    QGridLayout *gridLayout;

    void showBookDetailDialog(int id, const QString &title, const QString &author, const QString &category, int year, int quantity, double price);
};

#endif