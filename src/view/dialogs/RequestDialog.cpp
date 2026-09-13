#include "RequestDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>

RequestDialog::RequestDialog(const QString &username, QWidget *parent) 
    : QDialog(parent), currentUsername(username) {
    
    setWindowTitle("Gửi yêu cầu hệ thống (Send Request)");
    setFixedSize(400, 380);
    setStyleSheet("background-color: white; color: #1e293b; font-size: 13px;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    QLabel *titleLabel = new QLabel("Tạo Yêu Cầu Mới", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #2563eb;");
    mainLayout->addWidget(titleLabel);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(12);

    requestTypeCombo = new QComboBox(this);
    requestTypeCombo->addItem("Request borrow book (Mượn sách)");
    requestTypeCombo->addItem("Request to reserve book (Đặt trước sách)");
    requestTypeCombo->addItem("Request to renew book (Gia hạn sách)");
    requestTypeCombo->addItem("Request return book (Trả sách)");
    requestTypeCombo->addItem("Report issue of the book (Báo cáo sự cố)");
    requestTypeCombo->addItem("Pay fine (Thanh toán phạt)");
    requestTypeCombo->setStyleSheet("padding: 6px; border: 1px solid #cbd5e1; border-radius: 4px; background: white;");

    bookCombo = new QComboBox(this);
    bookCombo->setStyleSheet("padding: 6px; border: 1px solid #cbd5e1; border-radius: 4px; background: white;");
    
    noteEdit = new QTextEdit(this);
    noteEdit->setPlaceholderText("Nhập ghi chú hoặc mô tả chi tiết (nếu có)...");
    noteEdit->setFixedHeight(80);
    noteEdit->setStyleSheet("border: 1px solid #cbd5e1; border-radius: 4px; padding: 4px;");

    formLayout->addRow("Loại yêu cầu:", requestTypeCombo);
    formLayout->addRow("Chọn sách:", bookCombo);
    formLayout->addRow("Ghi chú / Mô tả:", noteEdit);

    mainLayout->addLayout(formLayout);

    loadUserBooks();

    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *submitBtn = new QPushButton("Gửi yêu cầu", this);
    QPushButton *cancelBtn = new QPushButton("Hủy", this);
    
    submitBtn->setStyleSheet("background-color: #2563eb; color: white; padding: 8px 16px; font-weight: bold; border-radius: 4px;");
    cancelBtn->setStyleSheet("background-color: #64748b; color: white; padding: 8px 16px; font-weight: bold; border-radius: 4px;");

    btnLayout->addWidget(submitBtn);
    btnLayout->addWidget(cancelBtn);
    mainLayout->addLayout(btnLayout);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(submitBtn, &QPushButton::clicked, this, &RequestDialog::handleSubmit);
}

void RequestDialog::loadUserBooks() {
    QSqlQuery query("SELECT id, title FROM books");
    while (query.next()) {
        int id = query.value(0).toInt();
        QString title = query.value(1).toString();
        bookCombo->addItem(QString("[%1] %2").arg(id).arg(title), id);
    }
}

void RequestDialog::handleSubmit() {
    QString reqType = requestTypeCombo->currentText();
    int bookId = bookCombo->currentData().toInt();
    QString currentDate = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    QSqlQuery userQuery;
    userQuery.prepare("SELECT id FROM users WHERE username = ?");
    userQuery.addBindValue(currentUsername);
    int userId = 1;
    if (userQuery.exec() && userQuery.next()) {
        userId = userQuery.value(0).toInt();
    }

    QSqlQuery query;
    query.prepare("INSERT INTO requests (user_id, book_id, type, request_date, status) VALUES (:user_id, :book_id, :type, :date, :status)");
    query.bindValue(":user_id", userId);
    query.bindValue(":book_id", bookId);
    query.bindValue(":type", reqType);
    query.bindValue(":date", currentDate);
    query.bindValue(":status", "Pending");

    if (query.exec()) {
        QMessageBox::information(this, "Thành công", "Gửi yêu cầu hệ thống thành công! Quản trị viên sẽ xử lý sớm.");
        accept();
    } else {
        QMessageBox::critical(this, "Lỗi", "Không thể lưu yêu cầu vào cơ sở dữ liệu!");
    }
}