#include "BorrowView.h"
#include <QVBoxLayout>
#include <QLabel>
BorrowView::BorrowView(QWidget *parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("🔄 Quản lý mượn/trả (Borrow UI)"));
}