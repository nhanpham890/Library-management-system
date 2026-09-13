#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Đăng nhập hệ thống");
    setFixedSize(350, 260);
    setStyleSheet("background-color: white;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(15);

    QLabel *titleLabel = new QLabel("<h2>Đăng Nhập</h2>");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    // Ô nhập Tài khoản
    userEdit = new QLineEdit();
    userEdit->setPlaceholderText("Tên tài khoản hoặc Email");
    userEdit->setStyleSheet("padding: 10px; border: 1px solid #ccc; border-radius: 5px; font-size: 13px;");
    layout->addWidget(userEdit);

    // Ô nhập Mật khẩu (ẩn ký tự)
    passEdit = new QLineEdit();
    passEdit->setPlaceholderText("Mật khẩu");
    passEdit->setEchoMode(QLineEdit::Password);
    passEdit->setStyleSheet("padding: 10px; border: 1px solid #ccc; border-radius: 5px; font-size: 13px;");
    layout->addWidget(passEdit);

    // Nút Đăng nhập
    QPushButton *loginBtn = new QPushButton("ĐĂNG NHẬP");
    loginBtn->setCursor(Qt::PointingHandCursor);
    loginBtn->setStyleSheet("background-color: #0052cc; color: white; padding: 10px; border-radius: 5px; font-weight: bold;");
    layout->addWidget(loginBtn);

    // Thanh tùy chọn phía dưới
    QHBoxLayout *optionsLayout = new QHBoxLayout();
    QPushButton *forgotBtn = new QPushButton("Quên mật khẩu?");
    forgotBtn->setFlat(true);
    forgotBtn->setStyleSheet("color: #0052cc; border: none; text-align: left;");
    
    QPushButton *registerBtn = new QPushButton("Đăng ký tài khoản");
    registerBtn->setFlat(true);
    registerBtn->setStyleSheet("color: #0052cc; border: none; text-align: right;");

    optionsLayout->addWidget(forgotBtn);
    optionsLayout->addStretch();
    optionsLayout->addWidget(registerBtn);
    
    layout->addLayout(optionsLayout);
    layout->addStretch();

    // Xử lý sự kiện bấm nút đăng nhập
    connect(loginBtn, &QPushButton::clicked, this, [this]() {
        if(userEdit->text().isEmpty() || passEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Lỗi", "Vui lòng nhập đầy đủ thông tin!");
        } else {
            this->accept(); // Đóng hộp thoại và trả về trạng thái Accepted
        }
    });
}

QString LoginDialog::getUsername() const {
    return userEdit->text();
}