#include "LoginDialog.h"
#include "RegisterDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Đăng nhập hệ thống");
    setFixedSize(360, 320);
    setStyleSheet("background-color: white;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(14);
    layout->setContentsMargins(25, 20, 25, 20);

    // Tiêu đề form Đăng nhập
    QLabel *titleLabel = new QLabel("ĐĂNG NHẬP");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #1a1a1a; margin-bottom: 5px;");
    layout->addWidget(titleLabel);

    // Ô nhập Tài khoản
    userEdit = new QLineEdit();
    userEdit->setPlaceholderText("Tên tài khoản");
    userEdit->setStyleSheet("padding: 10px; border: 1px solid #ccc; border-radius: 5px; font-size: 13px; color: black; background: white;");
    layout->addWidget(userEdit);

    // Hàng nhập Mật khẩu + Nút xem mật khẩu
    QHBoxLayout *passLayout = new QHBoxLayout();
    passLayout->setSpacing(6);

    passEdit = new QLineEdit();
    passEdit->setPlaceholderText("Mật khẩu");
    passEdit->setEchoMode(QLineEdit::Password);
    passEdit->setStyleSheet("padding: 10px; border: 1px solid #ccc; border-radius: 5px; font-size: 13px; color: black; background: white;");

    QPushButton *eyeBtn = new QPushButton("👁");
    eyeBtn->setFixedSize(42, 38);
    eyeBtn->setCursor(Qt::PointingHandCursor);
    eyeBtn->setStyleSheet(
        "QPushButton {"
        "   border: 1px solid #ccc; border-radius: 5px;"
        "   background-color: #f1f5f9; font-size: 16px;"
        "}"
        "QPushButton:hover { background-color: #e2e8f0; }"
    );

    // Chuyển đổi ẩn / hiện mật khẩu
    connect(eyeBtn, &QPushButton::clicked, this, [this, eyeBtn]() {
        if (passEdit->echoMode() == QLineEdit::Password) {
            passEdit->setEchoMode(QLineEdit::Normal);
            eyeBtn->setText("🙈");
        } else {
            passEdit->setEchoMode(QLineEdit::Password);
            eyeBtn->setText("👁");
        }
    });

    passLayout->addWidget(passEdit);
    passLayout->addWidget(eyeBtn);
    layout->addLayout(passLayout);

    // Nút Đăng nhập
    QPushButton *loginBtn = new QPushButton("ĐĂNG NHẬP");
    loginBtn->setCursor(Qt::PointingHandCursor);
    loginBtn->setStyleSheet("background-color: #0052cc; color: white; padding: 10px; border-radius: 5px; font-weight: bold; font-size: 13px;");
    layout->addWidget(loginBtn);

    // Tùy chọn phía dưới
    QHBoxLayout *optionsLayout = new QHBoxLayout();
    QPushButton *forgotBtn = new QPushButton("Quên mật khẩu?");
    forgotBtn->setFlat(true);
    forgotBtn->setCursor(Qt::PointingHandCursor);
    forgotBtn->setStyleSheet("color: #0052cc; border: none; text-align: left; font-size: 12px; background: transparent;");

    QPushButton *registerBtn = new QPushButton("Đăng ký tài khoản");
    registerBtn->setFlat(true);
    registerBtn->setCursor(Qt::PointingHandCursor);
    registerBtn->setStyleSheet("color: #0052cc; border: none; text-align: right; font-size: 12px; background: transparent;");

    optionsLayout->addWidget(forgotBtn);
    optionsLayout->addStretch();
    optionsLayout->addWidget(registerBtn);
    layout->addLayout(optionsLayout);

    // Hàm hiển thị thông báo với chữ màu đen rõ ràng
    auto showPopup = [this](const QString &title, const QString &content, QMessageBox::Icon icon) {
        QMessageBox box(this);
        box.setWindowTitle(title);
        box.setText(content);
        box.setIcon(icon);
        box.setStyleSheet(
            "QMessageBox { background-color: #ffffff; }"
            "QLabel { color: #000000; font-size: 13px; min-width: 200px; }"
            "QPushButton { background-color: #0052cc; color: #ffffff; padding: 6px 16px; border-radius: 4px; font-weight: bold; }"
        );
        box.exec();
    };

    // Kiểm tra đăng nhập với CSDL
    connect(loginBtn, &QPushButton::clicked, this, [this, showPopup]() {
        QString user = userEdit->text().trimmed();
        QString pass = passEdit->text().trimmed();

        if (user.isEmpty() || pass.isEmpty()) {
            showPopup("Lỗi", "Vui lòng nhập đầy đủ thông tin!", QMessageBox::Warning);
            return;
        }

        // 1. Kiểm tra trạng thái kết nối CSDL
        QSqlDatabase db = QSqlDatabase::database();
        if (!db.isOpen()) {
            showPopup("Lỗi CSDL", "Chưa thể kết nối CSDL:\n" + db.lastError().text(), QMessageBox::Critical);
            return;
        }

        // 2. Truy vấn người dùng
        QSqlQuery query;
        query.prepare("SELECT password FROM users WHERE username = :u");
        query.bindValue(":u", user);

        if (!query.exec()) {
            showPopup("Lỗi CSDL", "Truy vấn thất bại:\n" + query.lastError().text(), QMessageBox::Critical);
            return;
        }

        if (query.next()) {
            QString dbPassword = query.value(0).toString();
            if (dbPassword == pass) {
                this->accept();
            } else {
                showPopup("Lỗi", "Mật khẩu không chính xác!", QMessageBox::Warning);
            }
        } else {
            showPopup("Lỗi", "Tài khoản không tồn tại trong hệ thống!", QMessageBox::Warning);
        }
    });

    // Sự kiện Quên mật khẩu
    connect(forgotBtn, &QPushButton::clicked, this, [this, showPopup]() {
        QDialog forgotDialog(this);
        forgotDialog.setWindowTitle("Khôi phục mật khẩu");
        forgotDialog.setFixedSize(320, 190);
        forgotDialog.setStyleSheet("background-color: white;");

        QVBoxLayout *dlgLayout = new QVBoxLayout(&forgotDialog);
        dlgLayout->setSpacing(12);
        dlgLayout->setContentsMargins(20, 20, 20, 20);

        QLabel *fTitle = new QLabel("KHÔI PHỤC MẬT KHẨU");
        fTitle->setAlignment(Qt::AlignCenter);
        fTitle->setStyleSheet("font-size: 15px; font-weight: bold; color: #1a1a1a;");
        dlgLayout->addWidget(fTitle);

        QLineEdit *phoneEdit = new QLineEdit();
        phoneEdit->setPlaceholderText("SĐT");
        phoneEdit->setStyleSheet("padding: 9px; border: 1px solid #ccc; border-radius: 5px; font-size: 13px; color: black; background: white;");
        dlgLayout->addWidget(phoneEdit);

        QPushButton *confirmBtn = new QPushButton("XÁC NHẬN");
        confirmBtn->setCursor(Qt::PointingHandCursor);
        confirmBtn->setStyleSheet("background-color: #0052cc; color: white; padding: 9px; border-radius: 5px; font-weight: bold;");
        dlgLayout->addWidget(confirmBtn);

        connect(confirmBtn, &QPushButton::clicked, &forgotDialog, [&]() {
            QString phone = phoneEdit->text().trimmed();
            if (phone.isEmpty()) {
                showPopup("Lỗi", "Vui lòng nhập số điện thoại!", QMessageBox::Warning);
                return;
            }

            QSqlQuery query;
            query.prepare("SELECT username, password FROM users WHERE phone = :p");
            query.bindValue(":p", phone);

            if (query.exec() && query.next()) {
                showPopup("Khôi phục thành công", 
                          "Tài khoản: " + query.value(0).toString() + "\nMật khẩu: " + query.value(1).toString(), 
                          QMessageBox::Information);
                forgotDialog.accept();
            } else {
                showPopup("Thông báo", "Số điện thoại chưa được đăng ký trong hệ thống!", QMessageBox::Warning);
            }
        });

        forgotDialog.exec();
    });

    // Sự kiện Đăng ký tài khoản
    connect(registerBtn, &QPushButton::clicked, this, [this]() {
        RegisterDialog regDialog(this);
        regDialog.exec();
    });
}

QString LoginDialog::getUsername() const {
    return userEdit->text().trimmed();
}