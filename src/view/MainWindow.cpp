#include "MainWindow.h"
#include "dialogs/LoginDialog.h"
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), isLoggedIn(false) {
    setWindowTitle("Hệ thống Quản lý Thư viện - UTH");
    resize(1000, 700);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Khởi tạo các thành phần giao diện theo chuẩn Clean Architecture
    searchWidget = new SearchWidget(this);
    dashboardWidget = new DashboardWidget(this);
    bookListView = new BookListView(this);
    statusBarWidget = new StatusBar(this);

    mainLayout->addWidget(searchWidget);
    mainLayout->addWidget(dashboardWidget);
    mainLayout->addWidget(bookListView);
    mainLayout->addWidget(statusBarWidget);

    setCentralWidget(centralWidget);

    // 1. Kết nối tín hiệu đăng nhập/đăng xuất từ SearchWidget
    connect(searchWidget, &SearchWidget::loginButtonClicked, this, &MainWindow::handleLoginAction);

    // 2. Kết nối tín hiệu tìm kiếm đa tiêu chí (Search by title, author, subject, date) từ DashboardWidget sang BookListView
    connect(dashboardWidget, &DashboardWidget::searchTriggered, this, [this](const QString &criteria, const QString &keyword) {
        bookListView->searchBooks(criteria, keyword);
    });
}

void MainWindow::handleLoginAction() {
    if (!isLoggedIn) {
        LoginDialog loginDialog(this);
        if (loginDialog.exec() == QDialog::Accepted) {
            isLoggedIn = true;
            QString username = loginDialog.getUsername();
            if (username.isEmpty()) username = "Phạm Thành Nhân";
            searchWidget->updateUserInfo(username, true);
        }
    } else {
        isLoggedIn = false;
        searchWidget->updateUserInfo("Guest", false);
    }
}