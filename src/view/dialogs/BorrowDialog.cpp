#include "BorrowDialog.h"
#include <QVBoxLayout>
#include <QLabel>

BorrowDialog::BorrowDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Xác nhận Mượn sách");
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Giao diện xác nhận mượn sách ở đây"));
}