#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QLabel>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Đăng Nhập");
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Giao diện Đăng nhập ở đây"));
}