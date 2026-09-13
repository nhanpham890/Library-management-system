#include "MainWindow.h"
#include "widgets/DashboardWidget.h"
#include "widgets/BookListView.h"
#include "widgets/StatusBar.h"
#include "dialogs/LoginDialog.h"
#include "dialogs/RegisterDialog.h"
#include "dialogs/UserDetailDialog.h"
#include "dialogs/RequestDialog.h"
#include "dialogs/MailboxDialog.h" // Nhúng thư viện Hộp thư mới thêm
#include "widgets/AdminPanel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QProcess>
#include <QCoreApplication>
#include <QDir>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isLoggedIn(false) {
    
    setWindowTitle("Hệ thống Quản lý Thư viện - UTH");
    resize(1000, 700);
    setStyleSheet("background-color: #ffffff; color: #1e293b;");

    // Lệnh dự phòng: Đảm bảo cột status tồn tại trong bảng users (tránh lỗi nếu CSDL cũ chưa có)
    QSqlQuery alterQuery;
    alterQuery.exec("ALTER TABLE users ADD COLUMN status TEXT DEFAULT 'Active'");

    // Khôi phục phiên đăng nhập khi Reload App qua tham số dòng lệnh
    QStringList args = QCoreApplication::arguments();
    QString autoLoginUser = "";
    
    for (const QString &arg : args) {
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT username FROM users WHERE username = ?");
        checkQuery.addBindValue(arg.trimmed());
        if (checkQuery.exec() && checkQuery.next()) {
            autoLoginUser = checkQuery.value(0).toString().trimmed();
            break;
        }
    }

    if (!autoLoginUser.isEmpty()) {
        isLoggedIn = true;
    }

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    QHBoxLayout *headerLayout = new QHBoxLayout();
    
    QLabel *appTitle = new QLabel("Books Management", this);
    appTitle->setStyleSheet("font-size: 20px; font-weight: bold; color: #2563eb; background: transparent;");
    headerLayout->addWidget(appTitle);

    headerLayout->addStretch();

    // 1. Nút Hộp thư (Mailbox)
    QPushButton *mailboxBtn = new QPushButton("Hộp thư 📬", this);
    mailboxBtn->setCursor(Qt::PointingHandCursor);
    mailboxBtn->setVisible(isLoggedIn);
    mailboxBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #38bdf8;"
        "   color: #0c4a6e;"
        "   padding: 8px 14px;"
        "   border-radius: 6px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0ea5e9;"
        "   color: white;"
        "}"
    );
    headerLayout->addWidget(mailboxBtn);

    // 2. Nút Gửi yêu cầu (Send Request)
    QPushButton *sendRequestBtn = new QPushButton("Gửi yêu cầu", this);
    sendRequestBtn->setCursor(Qt::PointingHandCursor);
    sendRequestBtn->setVisible(isLoggedIn);
    sendRequestBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #059669;"
        "   color: white;"
        "   padding: 8px 14px;"
        "   border-radius: 6px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #047857;"
        "}"
    );
    headerLayout->addWidget(sendRequestBtn);

    // 3. Nút Quản lý Admin
    QPushButton *adminPanelBtn = new QPushButton("Quản lý (Admin)", this);
    adminPanelBtn->setCursor(Qt::PointingHandCursor);
    adminPanelBtn->setVisible(false);
    adminPanelBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #d97706;"
        "   color: white;"
        "   padding: 8px 14px;"
        "   border-radius: 6px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #b45309;"
        "}"
    );
    headerLayout->addWidget(adminPanelBtn);

    QString statusText = isLoggedIn ? ("Xin chào, " + autoLoginUser) : "Xin chào, Guest";
    userStatusBtn = new QPushButton(statusText, this);
    userStatusBtn->setCursor(Qt::PointingHandCursor);
    userStatusBtn->setStyleSheet("font-size: 13px; color: #64748b; background: transparent; border: none; font-weight: bold;");
    headerLayout->addWidget(userStatusBtn);

    QPushButton *reloadBtn = new QPushButton("Làm mới App", this);
    reloadBtn->setCursor(Qt::PointingHandCursor);
    reloadBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #64748b;"
        "   color: white;"
        "   padding: 8px 12px;"
        "   border-radius: 6px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #475569;"
        "}"
    );
    headerLayout->addWidget(reloadBtn);

    QString authBtnText = isLoggedIn ? "Đăng xuất" : "Đăng nhập / Đăng ký";
    authBtn = new QPushButton(authBtnText, this);
    authBtn->setCursor(Qt::PointingHandCursor);
    authBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #2563eb;"
        "   color: white;"
        "   padding: 8px 16px;"
        "   border-radius: 6px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #1d4ed8;"
        "}"
    );
    headerLayout->addWidget(authBtn);
    mainLayout->addLayout(headerLayout);

    // Bật hiển thị nút Admin nếu tài khoản đang phục hồi (Reload) là Admin
    if (isLoggedIn) {
        QSqlQuery roleQuery;
        roleQuery.prepare("SELECT role FROM users WHERE username = ?");
        roleQuery.addBindValue(autoLoginUser);
        if (roleQuery.exec() && roleQuery.next()) {
            if (roleQuery.value(0).toString().trimmed().toLower() == "admin") {
                adminPanelBtn->setVisible(true);
            }
        }
    }

    dashboardWidget = new DashboardWidget(this);
    mainLayout->addWidget(dashboardWidget);

    bookListView = new BookListView(this);
    mainLayout->addWidget(bookListView);

    statusBarWidget = new StatusBar(this);
    mainLayout->addWidget(statusBarWidget);

    // Sự kiện mở Hộp thư
    connect(mailboxBtn, &QPushButton::clicked, this, [=]() {
        QString currentUsername = userStatusBtn->text().replace("Xin chào, ", "").trimmed();
        MailboxDialog mailboxDlg(currentUsername, this);
        mailboxDlg.exec();
    });

    // Sự kiện Gửi yêu cầu (Chặn nếu tài khoản bị Đình chỉ - Suspended)
    connect(sendRequestBtn, &QPushButton::clicked, this, [=]() {
        QString currentUsername = userStatusBtn->text().replace("Xin chào, ", "").trimmed();

        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT status FROM users WHERE username = ?");
        checkQuery.addBindValue(currentUsername);
        if (checkQuery.exec() && checkQuery.next()) {
            QString status = checkQuery.value(0).toString().trimmed().toLower();
            if (status == "suspended") {
                QMessageBox::warning(this, "Truy cập bị từ chối", "Tài khoản của bạn đã bị đình chỉ và không thể gửi yêu cầu hệ thống!");
                return;
            }
        }

        RequestDialog reqDlg(currentUsername, this);
        reqDlg.exec();
    });

    // Sự kiện mở bảng Quản trị Admin
    connect(adminPanelBtn, &QPushButton::clicked, this, [=]() {
        AdminPanel *adminPanel = new AdminPanel();
        adminPanel->resize(950, 550);
        
        connect(adminPanel, &AdminPanel::dataChanged, [=]() {
            bookListView->loadBooksFromDatabase();
        });

        adminPanel->show();
    });

    // Xử lý Đăng nhập / Đăng xuất an toàn
    connect(authBtn, &QPushButton::clicked, this, [=]() {
        if (!isLoggedIn) {
            LoginDialog loginDlg(this);
            if (loginDlg.exec() == QDialog::Accepted) {
                QString username = loginDlg.getUsername().trimmed();
                
                // Lấy vai trò (role)
                QSqlQuery query;
                query.prepare("SELECT role FROM users WHERE username = ?");
                query.addBindValue(username);
                QString role = "Member";
                if (query.exec() && query.next()) {
                    role = query.value(0).toString().trimmed();
                }

                // Kiểm tra trạng thái khóa (status)
                QSqlQuery statusQuery;
                statusQuery.prepare("SELECT status FROM users WHERE username = ?");
                statusQuery.addBindValue(username);
                if (statusQuery.exec() && statusQuery.next()) {
                    QString status = statusQuery.value(0).toString().trimmed().toLower();
                    if (status == "suspended") {
                        QMessageBox::warning(this, "Đăng nhập thất bại", "Tài khoản này đã bị đình chỉ hoạt động!");
                        return;
                    }
                }

                // Đăng nhập thành công, bật các nút tính năng
                isLoggedIn = true;
                userStatusBtn->setText("Xin chào, " + username);
                authBtn->setText("Đăng xuất");
                sendRequestBtn->setVisible(true);
                mailboxBtn->setVisible(true);

                if (role.toLower() == "admin") {
                    adminPanelBtn->setVisible(true);
                    QMessageBox::information(this, "Thông báo", "Đăng nhập thành công với tư cách Admin!");
                } else {
                    adminPanelBtn->setVisible(false);
                    QMessageBox::information(this, "Thông báo", "Đăng nhập thành công với tư cách Độc giả!");
                }
            }
        } else {
            // Xử lý khi đăng xuất
            isLoggedIn = false;
            adminPanelBtn->setVisible(false);
            sendRequestBtn->setVisible(false);
            mailboxBtn->setVisible(false);
            userStatusBtn->setText("Xin chào, Guest");
            authBtn->setText("Đăng nhập / Đăng ký");
            bookListView->loadBooksFromDatabase();
        }
    });

    // Sự kiện Làm mới App giữ nguyên phiên đăng nhập
    connect(reloadBtn, &QPushButton::clicked, this, [=]() {
        QString program = QCoreApplication::applicationFilePath();
        QStringList arguments;
        arguments.append(program);

        if (isLoggedIn) {
            QString currentUsername = userStatusBtn->text().replace("Xin chào, ", "").trimmed();
            arguments.append(currentUsername);
        }

        QString workingDirectory = QDir::currentPath();
        QProcess::startDetached(program, arguments, workingDirectory);
        QCoreApplication::quit();
    });

    // Sự kiện xem thông tin tài khoản cá nhân
    connect(userStatusBtn, &QPushButton::clicked, this, [=]() {
        if (!isLoggedIn) {
            QMessageBox::information(this, "Thông báo", "Bạn chưa đăng nhập tài khoản!");
            return;
        }
        QString currentUsername = userStatusBtn->text().replace("Xin chào, ", "").trimmed();
        UserDetailDialog detailDlg(currentUsername, this);
        detailDlg.exec();
    });

    // Sự kiện Tìm kiếm sách
    connect(dashboardWidget, &DashboardWidget::searchTriggered, this, [=](const QString &criteria, const QString &keyword) {
        bookListView->loadBooksFromDatabase(keyword, criteria);
        statusBarWidget->setStatusMessage(QString("Đang tìm kiếm theo: %1 với từ khóa: \"%2\"").arg(criteria).arg(keyword));
    });
}

MainWindow::~MainWindow() {}