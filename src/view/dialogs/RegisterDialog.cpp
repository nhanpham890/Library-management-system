#include "RegisterDialog.h"
#include <QVBoxLayout>
#include <QLabel>

RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Đăng Ký Tài Khoản");
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Giao diện Đăng ký ở đây"));
}