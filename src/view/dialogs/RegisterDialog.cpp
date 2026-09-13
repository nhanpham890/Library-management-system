#include "RegisterDialog.h"
#include "../../controller/LoginController.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Đăng ký tài khoản");
    setFixedSize(360, 360);
    setStyleSheet("background-color: white;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(12);
    layout->setContentsMargins(25, 20, 25, 20);

    QLabel *titleLabel = new QLabel("ĐĂNG KÝ");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #1a1a1a; margin-bottom: 5px;");
    layout->addWidget(titleLabel);

    QLineEdit *userEdit = new QLineEdit();
    userEdit->setPlaceholderText("Tên tài khoản");
    userEdit->setStyleSheet("padding: 9px; border: 1px solid #ccc; border-radius: 5px; font-size: 13px; color: black; background: white;");
    layout->addWidget(userEdit);

    QLineEdit *passEdit = new QLineEdit();
    passEdit->setPlaceholderText("Mật khẩu");
    passEdit->setEchoMode(QLineEdit::Password);
    passEdit->setStyleSheet("padding: 9px; border: 1px solid #ccc; border-radius: 5px; font-size: 13px; color: black; background: white;");
    layout->addWidget(passEdit);

    QLineEdit *emailEdit = new QLineEdit();
    emailEdit->setPlaceholderText("Email");
    emailEdit->setStyleSheet("padding: 9px; border: 1px solid #ccc; border-radius: 5px; font-size: 13px; color: black; background: white;");
    layout->addWidget(emailEdit);

    QLineEdit *phoneEdit = new QLineEdit();
    phoneEdit->setPlaceholderText("Số điện thoại");
    phoneEdit->setStyleSheet("padding: 9px; border: 1px solid #ccc; border-radius: 5px; font-size: 13px; color: black; background: white;");
    layout->addWidget(phoneEdit);

    QPushButton *registerBtn = new QPushButton("ĐĂNG KÝ");
    registerBtn->setCursor(Qt::PointingHandCursor);
    registerBtn->setStyleSheet("background-color: #0052cc; color: white; padding: 10px; border-radius: 5px; font-weight: bold; font-size: 13px;");
    layout->addWidget(registerBtn);

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

    connect(registerBtn, &QPushButton::clicked, this, [=]() {
        QString user = userEdit->text().trimmed();
        QString pass = passEdit->text().trimmed();
        QString email = emailEdit->text().trimmed();
        QString phone = phoneEdit->text().trimmed();

        if (user.isEmpty() || pass.isEmpty() || email.isEmpty() || phone.isEmpty()) {
            showPopup("Lỗi", "Vui lòng điền đầy đủ các mục thông tin!", QMessageBox::Warning);
            return;
        }

        if (LoginController::registerUser(user, pass, email, phone)) {
            showPopup("Thành công", "Đăng ký thành công! Dữ liệu đã lưu vào cơ sở dữ liệu.", QMessageBox::Information);
            this->accept();
        } else {
            showPopup("Lỗi", "Tên tài khoản đã tồn tại hoặc xảy ra lỗi khi lưu vào cơ sở dữ liệu!", QMessageBox::Warning);
        }
    });
}