#include "AdminPanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>

AdminPanel::AdminPanel(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Hệ thống Quản trị Thư viện (Admin Dashboard)");
    resize(950, 550);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    tabWidget = new QTabWidget(this);
    
    setupMembersTab();
    setupBooksTab();
    setupRequestsTab();

    tabWidget->addTab(membersTab, "👤 Quản lý Độc giả");
    tabWidget->addTab(booksTab, "📚 Quản lý Sách");
    tabWidget->addTab(requestsTab, "📝 Quản lý Yêu cầu");

    mainLayout->addWidget(tabWidget);
    setupStyles();

    loadUserData();
    loadBookData();
    loadRequestData();
}

void AdminPanel::setupMembersTab() {
    membersTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(membersTab);

    memberTable = new QTableWidget();
    memberTable->setColumnCount(6);
    memberTable->setHorizontalHeaderLabels({"ID", "Tên tài khoản", "Gmail", "SĐT", "Vai trò", "Trạng thái"});
    memberTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    memberTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    memberTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(memberTable);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    refreshMemberBtn = new QPushButton("Làm mới");
    viewMemberBtn = new QPushButton("Xem chi tiết");
    addMemberBtn = new QPushButton("Thêm thành viên");
    suspendMemberBtn = new QPushButton("Đình chỉ (Suspend)");
    deleteMemberBtn = new QPushButton("Xóa thành viên");

    deleteMemberBtn->setObjectName("dangerBtn");
    suspendMemberBtn->setObjectName("warningBtn");

    btnLayout->addWidget(refreshMemberBtn);
    btnLayout->addWidget(viewMemberBtn);
    btnLayout->addWidget(addMemberBtn);
    btnLayout->addWidget(suspendMemberBtn);
    btnLayout->addWidget(deleteMemberBtn);
    layout->addLayout(btnLayout);

    connect(refreshMemberBtn, &QPushButton::clicked, this, &AdminPanel::loadUserData);
    connect(deleteMemberBtn, &QPushButton::clicked, this, &AdminPanel::deleteSelectedUser);
}

void AdminPanel::setupBooksTab() {
    booksTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(booksTab);

    bookTable = new QTableWidget();
    bookTable->setColumnCount(7);
    bookTable->setHorizontalHeaderLabels({"ID Sách", "Tên Sách", "Tác giả", "Thể loại", "Năm XB", "Số lượng", "Giá ($)"});
    bookTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    bookTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    bookTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(bookTable);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    addBookBtn = new QPushButton("Thêm sách (Add)");
    editBookBtn = new QPushButton("Sửa sách (Edit)");
    removeBookBtn = new QPushButton("Xóa sách (Remove)");
    
    removeBookBtn->setObjectName("dangerBtn");

    btnLayout->addStretch();
    btnLayout->addWidget(addBookBtn);
    btnLayout->addWidget(editBookBtn);
    btnLayout->addWidget(removeBookBtn);
    layout->addLayout(btnLayout);

    connect(addBookBtn, &QPushButton::clicked, this, &AdminPanel::addBook);
    connect(editBookBtn, &QPushButton::clicked, this, &AdminPanel::editBook);
    connect(removeBookBtn, &QPushButton::clicked, this, &AdminPanel::removeBook);
}

void AdminPanel::setupRequestsTab() {
    requestsTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(requestsTab);

    requestTable = new QTableWidget();
    requestTable->setColumnCount(6);
    requestTable->setHorizontalHeaderLabels({"Mã YC", "Độc giả", "Tên sách", "Loại YC", "Ngày tạo", "Trạng thái"});
    requestTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    requestTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    requestTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(requestTable);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    approveBorrowBtn = new QPushButton("Duyệt Mượn (Borrow)");
    approveReserveBtn = new QPushButton("Duyệt Đặt trước (Reserve)");
    approveRenewBtn = new QPushButton("Duyệt Gia hạn (Renew)");
    approveReturnBtn = new QPushButton("Duyệt Trả & Phạt (Return & Fine)");

    approveReturnBtn->setObjectName("warningBtn");

    btnLayout->addWidget(approveBorrowBtn);
    btnLayout->addWidget(approveReserveBtn);
    btnLayout->addWidget(approveRenewBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(approveReturnBtn);
    layout->addLayout(btnLayout);

    // Kết nối sự kiện cho các nút xử lý yêu cầu
    connect(approveBorrowBtn, &QPushButton::clicked, this, [=]() { handleApproveRequest("Borrow"); });
    connect(approveReserveBtn, &QPushButton::clicked, this, [=]() { handleApproveRequest("Reserve"); });
    connect(approveRenewBtn, &QPushButton::clicked, this, [=]() { handleApproveRequest("Renew"); });
    connect(approveReturnBtn, &QPushButton::clicked, this, [=]() { handleApproveRequest("Return & Fine"); });
}

void AdminPanel::setupStyles() {
    this->setStyleSheet(
        "QTabWidget::pane { border: 1px solid #cbd5e1; background: white; border-radius: 5px; }"
        "QTabBar::tab { background: #f1f5f9; color: #475569; padding: 10px 20px; border-right: 1px solid #cbd5e1; border-bottom: 1px solid #cbd5e1; font-weight: bold; }"
        "QTabBar::tab:selected { background: white; color: #2563eb; border-bottom: none; }"
        
        "QTableWidget { background-color: white; color: #0f172a; gridline-color: #e2e8f0; font-size: 13px; }"
        "QHeaderView::section { background-color: #f8fafc; color: #0f172a; font-weight: bold; padding: 5px; border: 1px solid #e2e8f0; }"
        
        "QPushButton { background-color: #2563eb; color: white; padding: 8px 14px; border-radius: 5px; font-weight: bold; }"
        "QPushButton:hover { background-color: #1d4ed8; }"
        
        "QPushButton#dangerBtn { background-color: #dc2626; }"
        "QPushButton#dangerBtn:hover { background-color: #b91c1c; }"
        
        "QPushButton#warningBtn { background-color: #d97706; }"
        "QPushButton#warningBtn:hover { background-color: #b45309; }"
    );
}

void AdminPanel::loadUserData() {
    memberTable->setRowCount(0);
    QSqlQuery query("SELECT id, username, email, phone, role FROM users ORDER BY id ASC");
    int row = 0;
    while (query.next()) {
        memberTable->insertRow(row);
        for (int col = 0; col < 5; ++col) {
            memberTable->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
       
        QTableWidgetItem *statusItem = new QTableWidgetItem("Hoạt động");
        statusItem->setForeground(QBrush(Qt::darkGreen));
        memberTable->setItem(row, 5, statusItem);
        row++;
    }
}

void AdminPanel::loadBookData() {
    bookTable->setRowCount(0);
    QSqlQuery query("SELECT id, title, author, category, year, quantity, price FROM books ORDER BY id ASC");
    int row = 0;
    while (query.next()) {
        bookTable->insertRow(row);
        for (int col = 0; col < 7; ++col) {
            bookTable->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }
}

void AdminPanel::loadRequestData() {
    requestTable->setRowCount(0);
    
    // Thêm điều kiện WHERE status = 'Pending' để chỉ hiển thị các yêu cầu chưa duyệt
    QSqlQuery query("SELECT id, user_id, book_id, type, request_date, status FROM requests WHERE status = 'Pending' ORDER BY id ASC");
    
    int row = 0;
    while (query.next()) {
        requestTable->insertRow(row);
        for (int col = 0; col < 6; ++col) {
            requestTable->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }
}

void AdminPanel::deleteSelectedUser() {
    int currentRow = memberTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một thành viên để xóa!");
        return;
    }

    QString id = memberTable->item(currentRow, 0)->text();
    QString username = memberTable->item(currentRow, 1)->text();

    if (username == "admin") {
        QMessageBox::warning(this, "Từ chối", "Không thể xóa tài khoản Quản trị viên gốc!");
        return;
    }

    if (QMessageBox::question(this, "Xác nhận", "Xóa vĩnh viễn tài khoản: " + username + "?") == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM users WHERE id = :id");
        query.bindValue(":id", id);
        if (query.exec()) {
            loadUserData();
        }
    }
}

void AdminPanel::handleApproveRequest(const QString &actionType) {
    int currentRow = requestTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một yêu cầu trong bảng để xử lý!");
        return;
    }

    QString requestId = requestTable->item(currentRow, 0)->text();
    QString currentStatus = requestTable->item(currentRow, 5)->text();

    if (currentStatus == "Approved" || currentStatus == "Completed") {
        QMessageBox::information(this, "Thông báo", "Yêu cầu này đã được xử lý trước đó rồi!");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE requests SET status = 'Approved' WHERE id = :id");
    query.bindValue(":id", requestId);

    if (query.exec()) {
        QMessageBox::information(this, "Thành công", QString("Đã duyệt yêu cầu thành công (%1)!").arg(actionType));
        loadRequestData();
        emit dataChanged();
    } else {
        QMessageBox::critical(this, "Lỗi", "Không thể cập nhật trạng thái yêu cầu trong cơ sở dữ liệu!");
    }
}

void AdminPanel::addBook() {
    QDialog dialog(this);
    dialog.setWindowTitle("Thêm sách mới");
    dialog.setFixedSize(360, 340);
    dialog.setStyleSheet("background-color: white; color: #1e293b;");

    QFormLayout form(&dialog);

    QLineEdit titleEdit;
    QLineEdit authorEdit;
    QLineEdit categoryEdit;
    QSpinBox yearSpin;
    yearSpin.setRange(1800, 2100);
    yearSpin.setValue(2026);
    QSpinBox qtySpin;
    qtySpin.setRange(0, 10000);
    qtySpin.setValue(1);
    
    QDoubleSpinBox priceSpin;
    priceSpin.setRange(0.0, 100000.0);
    priceSpin.setDecimals(2);
    priceSpin.setValue(10.0);

    form.addRow("Tên sách:", &titleEdit);
    form.addRow("Tác giả:", &authorEdit);
    form.addRow("Thể loại:", &categoryEdit);
    form.addRow("Năm XB:", &yearSpin);
    form.addRow("Số lượng:", &qtySpin);
    form.addRow("Giá ($):", &priceSpin);

    QHBoxLayout btnLayout;
    QPushButton saveBtn("Lưu");
    QPushButton cancelBtn("Hủy");
    saveBtn.setStyleSheet("background-color: #2563eb; color: white; padding: 6px 12px; font-weight: bold; border-radius: 4px;");
    cancelBtn.setStyleSheet("background-color: #64748b; color: white; padding: 6px 12px; font-weight: bold; border-radius: 4px;");
    
    btnLayout.addWidget(&saveBtn);
    btnLayout.addWidget(&cancelBtn);
    form.addRow(&btnLayout);

    connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(&saveBtn, &QPushButton::clicked, [&]() {
        if (titleEdit.text().trimmed().isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Tên sách không được để trống!");
            return;
        }

        QSqlQuery query;
        query.prepare("INSERT INTO books (title, author, category, year, quantity, price) VALUES (:title, :author, :category, :year, :quantity, :price)");
        query.bindValue(":title", titleEdit.text().trimmed());
        query.bindValue(":author", authorEdit.text().trimmed());
        query.bindValue(":category", categoryEdit.text().trimmed());
        query.bindValue(":year", yearSpin.value());
        query.bindValue(":quantity", qtySpin.value());
        query.bindValue(":price", priceSpin.value());

        if (query.exec()) {
            loadBookData();
            emit dataChanged();
            dialog.accept();
        } else {
            QMessageBox::critical(&dialog, "Lỗi", "Không thể thêm sách vào CSDL!");
        }
    });

    dialog.exec();
}

void AdminPanel::editBook() {
    int currentRow = bookTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một cuốn sách để sửa!");
        return;
    }

    QString id = bookTable->item(currentRow, 0)->text();
    QString currentTitle = bookTable->item(currentRow, 1)->text();
    QString currentAuthor = bookTable->item(currentRow, 2)->text();
    QString currentCategory = bookTable->item(currentRow, 3)->text();
    int currentYear = bookTable->item(currentRow, 4)->text().toInt();
    int currentQty = bookTable->item(currentRow, 5)->text().toInt();
    double currentPrice = bookTable->item(currentRow, 6)->text().toDouble();

    QDialog dialog(this);
    dialog.setWindowTitle("Sửa thông tin sách");
    dialog.setFixedSize(360, 340);
    dialog.setStyleSheet("background-color: white; color: #1e293b;");

    QFormLayout form(&dialog);

    QLineEdit titleEdit(currentTitle);
    QLineEdit authorEdit(currentAuthor);
    QLineEdit categoryEdit(currentCategory);
    QSpinBox yearSpin;
    yearSpin.setRange(1800, 2100);
    yearSpin.setValue(currentYear > 0 ? currentYear : 2026);
    QSpinBox qtySpin;
    qtySpin.setRange(0, 10000);
    qtySpin.setValue(currentQty);

    QDoubleSpinBox priceSpin;
    priceSpin.setRange(0.0, 100000.0);
    priceSpin.setDecimals(2);
    priceSpin.setValue(currentPrice);

    form.addRow("Tên sách:", &titleEdit);
    form.addRow("Tác giả:", &authorEdit);
    form.addRow("Thể loại:", &categoryEdit);
    form.addRow("Năm XB:", &yearSpin);
    form.addRow("Số lượng:", &qtySpin);
    form.addRow("Giá ($):", &priceSpin);

    QHBoxLayout btnLayout;
    QPushButton saveBtn("Cập nhật");
    QPushButton cancelBtn("Hủy");
    saveBtn.setStyleSheet("background-color: #2563eb; color: white; padding: 6px 12px; font-weight: bold; border-radius: 4px;");
    cancelBtn.setStyleSheet("background-color: #64748b; color: white; padding: 6px 12px; font-weight: bold; border-radius: 4px;");
    
    btnLayout.addWidget(&saveBtn);
    btnLayout.addWidget(&cancelBtn);
    form.addRow(&btnLayout);

    connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(&saveBtn, &QPushButton::clicked, [&]() {
        if (titleEdit.text().trimmed().isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Tên sách không được để trống!");
            return;
        }

        QSqlQuery query;
        query.prepare("UPDATE books SET title = :title, author = :author, category = :category, year = :year, quantity = :quantity, price = :price WHERE id = :id");
        query.bindValue(":title", titleEdit.text().trimmed());
        query.bindValue(":author", authorEdit.text().trimmed());
        query.bindValue(":category", categoryEdit.text().trimmed());
        query.bindValue(":year", yearSpin.value());
        query.bindValue(":quantity", qtySpin.value());
        query.bindValue(":price", priceSpin.value());
        query.bindValue(":id", id);

        if (query.exec()) {
            loadBookData();
            emit dataChanged();
            dialog.accept();
        } else {
            QMessageBox::critical(&dialog, "Lỗi", "Không thể cập nhật thông tin sách!");
        }
    });

    dialog.exec();
}

void AdminPanel::removeBook() {
    int currentRow = bookTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một cuốn sách để xóa!");
        return;
    }

    QString id = bookTable->item(currentRow, 0)->text();
    QString title = bookTable->item(currentRow, 1)->text();

    if (QMessageBox::question(this, "Xác nhận", "Bạn có chắc chắn muốn xóa cuốn sách: " + title + "?") == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM books WHERE id = :id");
        query.bindValue(":id", id);
        if (query.exec()) {
            loadBookData();
            emit dataChanged();
        } else {
            QMessageBox::critical(this, "Lỗi", "Không thể xóa sách khỏi cơ sở dữ liệu!");
        }
    }
}