#include "MailboxDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>

MailboxDialog::MailboxDialog(const QString &username, QWidget *parent) 
    : QDialog(parent), currentUsername(username) {
    
    setWindowTitle("Hộp thư thông báo");
    setFixedSize(450, 400);
    setStyleSheet("background-color: white; color: #1e293b;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel *titleLabel = new QLabel("📬 Hộp thư của bạn", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #2563eb;");
    layout->addWidget(titleLabel);

    listWidget = new QListWidget(this);
    listWidget->setStyleSheet(
        "QListWidget { border: 1px solid #cbd5e1; border-radius: 6px; padding: 5px; background-color: #f8fafc; }"
        "QListWidget::item { border-bottom: 1px solid #e2e8f0; padding: 10px; }"
        "QListWidget::item:selected { background-color: #e0f2fe; color: #0369a1; }"
    );
    listWidget->setWordWrap(true); // Cho phép tin nhắn dài tự xuống dòng
    layout->addWidget(listWidget);

    loadMessages();

    QPushButton *closeBtn = new QPushButton("Đóng", this);
    closeBtn->setCursor(Qt::PointingHandCursor);
    closeBtn->setStyleSheet("background-color: #64748b; color: white; padding: 8px; border-radius: 4px; font-weight: bold;");
    layout->addWidget(closeBtn);

    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
}

void MailboxDialog::loadMessages() {
    QSqlQuery query;
    // Lấy tin nhắn của user hiện tại, sắp xếp tin mới nhất lên đầu
    query.prepare("SELECT message, created_at FROM notifications WHERE user_id = (SELECT id FROM users WHERE username = ?) ORDER BY id DESC");
    query.addBindValue(currentUsername);
    
    if (query.exec()) {
        while (query.next()) {
            QString msg = query.value(0).toString();
            QString date = query.value(1).toString();
            
            // Format tin nhắn hiển thị
            QString displayItem = QString("🕒 %1\n%2").arg(date, msg);
            listWidget->addItem(displayItem);
        }
    }

    if (listWidget->count() == 0) {
        listWidget->addItem("Bạn không có thông báo nào mới.");
    }
}