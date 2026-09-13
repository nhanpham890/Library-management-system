#include "SearchWidget.h"
#include <QHBoxLayout>

SearchWidget::SearchWidget(QWidget *parent) : QWidget(parent), isLoggedIn(false) {
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);

    logoLabel = new QLabel("<h2>📚 Books Management</h2>");
    logoLabel->setStyleSheet("color: #4a76a8;");
    layout->addWidget(logoLabel);

    layout->addStretch();

    userLabel = new QLabel("Xin chào, <b>Guest</b>");
    userLabel->setStyleSheet("font-size: 14px; color: #333; margin-right: 10px;");
    
    loginBtn = new QPushButton("Đăng nhập / Đăng ký");
    loginBtn->setCursor(Qt::PointingHandCursor);
    loginBtn->setStyleSheet("QPushButton { padding: 8px 15px; background-color: #0052cc; color: white; border-radius: 5px; font-weight: bold; }"
                            "QPushButton:hover { background-color: #0047b3; }");

    layout->addWidget(userLabel);
    layout->addWidget(loginBtn);

    connect(loginBtn, &QPushButton::clicked, this, [this]() {
        emit loginButtonClicked();
    });
}

void SearchWidget::updateUserInfo(const QString& username, bool loggedIn) {
    isLoggedIn = loggedIn;
    if (isLoggedIn) {
        userLabel->setText("Xin chào, <b>" + username + "</b>");
        loginBtn->setText("Đăng xuất");
        loginBtn->setStyleSheet("QPushButton { padding: 8px 15px; background-color: #d32f2f; color: white; border-radius: 5px; font-weight: bold; }");
    } else {
        userLabel->setText("Xin chào, <b>Guest</b>");
        loginBtn->setText("Đăng nhập / Đăng ký");
        loginBtn->setStyleSheet("QPushButton { padding: 8px 15px; background-color: #0052cc; color: white; border-radius: 5px; font-weight: bold; }");
    }
}