#include "BookListView.h"
#include <QVBoxLayout>
#include <QLabel>

BookListView::BookListView(QWidget *parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("📚 Danh sách sách (Book List Display)"));
}