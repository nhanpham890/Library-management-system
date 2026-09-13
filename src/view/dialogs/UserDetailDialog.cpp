#include "UserDetailDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QSqlQuery>

UserDetailDialog::UserDetailDialog(const QString &username, QWidget *parent) : QDialog(parent) {
    setWindowTitle("Thông tin tài khoản cá nhân");
    setFixedSize(360, 280);
    setStyleSheet("background-color: white; color: #1e293b; font-size: 13px;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    QLabel *titleLabel = new QLabel("Hồ sơ cá nhân", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #2563eb;");
    mainLayout->addWidget(titleLabel);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(12);

    lblUsername = new QLabel(this);
    lblEmail = new QLabel(this);
    lblPhone = new QLabel(this);
    lblRole = new QLabel(this);

    formLayout->addRow("Tên tài khoản:", lblUsername);
    formLayout->addRow("Email:", lblEmail);
    formLayout->addRow("Số điện thoại:", lblPhone);
    formLayout->addRow("Vai trò:", lblRole);

    mainLayout->addLayout(formLayout);

    loadUserInfo(username);

    mainLayout->addStretch();

    QPushButton *closeBtn = new QPushButton("Đóng", this);
    closeBtn->setCursor(Qt::PointingHandCursor);
    closeBtn->setStyleSheet("background-color: #2563eb; color: white; padding: 8px; font-weight: bold; border-radius: 4px;");
    mainLayout->addWidget(closeBtn);

    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
}

void UserDetailDialog::loadUserInfo(const QString &username) {
    QSqlQuery query;
    query.prepare("SELECT username, email, phone, role FROM users WHERE username = ?");
    query.addBindValue(username);

    if (query.exec() && query.next()) {
        lblUsername->setText(query.value(0).toString());
        lblEmail->setText(query.value(1).toString().isEmpty() ? "Chưa cập nhật" : query.value(1).toString());
        lblPhone->setText(query.value(2).toString().isEmpty() ? "Chưa cập nhật" : query.value(2).toString());
        lblRole->setText(query.value(3).toString());
    }
}