#include "MainWindow.h"
#include "BookCard.h"
#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), isLoggedIn(false) {
    setWindowTitle("Thư viện Quản lý Sách");
    resize(1000, 700);
    setupUI();
    loadBooks();
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setStyleSheet("background-color: #f5f7f9;");

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // --- Header ---
    QHBoxLayout* headerLayout = new QHBoxLayout();
    QLabel* logoLabel = new QLabel("<h2>📚 Books</h2>");
    logoLabel->setStyleSheet("color: #4a76a8;");
    headerLayout->addWidget(logoLabel);
    headerLayout->addStretch(); 

    userLabel = new QLabel("Xin chào, <b>Guest</b>");
    userLabel->setStyleSheet("font-size: 14px; color: #333; margin-right: 10px;");
    
    loginBtn = new QPushButton("Đăng nhập / Đăng ký");
    loginBtn->setCursor(Qt::PointingHandCursor);
    loginBtn->setStyleSheet("QPushButton { padding: 8px 15px; background-color: #0052cc; color: white; border-radius: 5px; font-weight: bold; } QPushButton:hover { background-color: #0047b3; }");
    
    headerLayout->addWidget(userLabel);
    headerLayout->addWidget(loginBtn);
    mainLayout->addLayout(headerLayout);

    connect(loginBtn, &QPushButton::clicked, this, &MainWindow::handleLoginLogout);

    // --- Grid Khu vực Sách ---
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; }");

    QWidget* scrollContent = new QWidget();
    gridLayout = new QGridLayout(scrollContent);
    gridLayout->setSpacing(20);
    gridLayout->setContentsMargins(20, 20, 20, 20);

    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);
}

void MainWindow::loadBooks() {
    struct BookData { QString title, qty, cat, price, color; };
    QList<BookData> books = {
        {"The Catcher in the Rye\nnovel by J.D. Salinger", "6", "Anthologies & Collections", "8.99", "#e0e0e0"},
        {"Atomic Habits", "6", "Non-Fiction", "11.98", "#ffe0b2"},
        {"Harry Potter", "2", "Poetry and Drama", "9.99", "#e1bee7"},
        {"The Lean Startup", "7", "Business & Economics", "16", "#ffcdd2"},
        {"Educated: A Memoir", "12", "Academic/Reference", "9", "#b3e5fc"}
    };

    for(int i = 0; i < books.size(); ++i) {
        int row = i / 4; 
        int col = i % 4; 
        BookCard* card = new BookCard(books[i].title, books[i].qty, books[i].cat, books[i].price, books[i].color);
        gridLayout->addWidget(card, row, col);
    }
    gridLayout->setRowStretch(gridLayout->rowCount(), 1);
}

void MainWindow::handleLoginLogout() {
    if (!isLoggedIn) {
        LoginDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            isLoggedIn = true;
            QString inputName = dialog.getUsername();
            userLabel->setText("Xin chào, <b>" + inputName + "</b>");
            loginBtn->setText("Đăng xuất");
            loginBtn->setStyleSheet("QPushButton { padding: 8px 15px; background-color: #d32f2f; color: white; border-radius: 5px; font-weight: bold; }");
        }
    } else {
        isLoggedIn = false;
        userLabel->setText("Xin chào, <b>Guest</b>");
        loginBtn->setText("Đăng nhập / Đăng ký");
        loginBtn->setStyleSheet("QPushButton { padding: 8px 15px; background-color: #0052cc; color: white; border-radius: 5px; font-weight: bold; }");
    }
}   