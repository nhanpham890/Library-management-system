#include "AdminPanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QDateTime>

AdminPanel::AdminPanel(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Hệ thống Quản trị Thư viện (Admin Dashboard)");
    resize(1000, 550); 
    
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
    unsuspendMemberBtn = new QPushButton("Mở khóa (Unsuspend)"); 
    deleteMemberBtn = new QPushButton("Xóa thành viên");

    deleteMemberBtn->setObjectName("dangerBtn");      
    suspendMemberBtn->setObjectName("warningBtn");    
    unsuspendMemberBtn->setObjectName("safeBtn");     

    btnLayout->addWidget(refreshMemberBtn);
    btnLayout->addWidget(viewMemberBtn);
    btnLayout->addWidget(addMemberBtn);
    btnLayout->addWidget(suspendMemberBtn);
    btnLayout->addWidget(unsuspendMemberBtn);
    btnLayout->addWidget(deleteMemberBtn);
    layout->addLayout(btnLayout);

    connect(refreshMemberBtn, &QPushButton::clicked, this, &AdminPanel::loadUserData);
    connect(viewMemberBtn, &QPushButton::clicked, this, &AdminPanel::viewSelectedUserDetail);
    connect(addMemberBtn, &QPushButton::clicked, this, &AdminPanel::addNewMember);
    connect(suspendMemberBtn, &QPushButton::clicked, this, &AdminPanel::suspendSelectedUser);
    connect(unsuspendMemberBtn, &QPushButton::clicked, this, &AdminPanel::unsuspendSelectedUser); 
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
    
    // Khởi tạo một nút duy nhất để duyệt
    approveRequestBtn = new QPushButton("Duyệt yêu cầu (Approve)");
    approveRequestBtn->setObjectName("safeBtn"); // Dùng màu xanh lá nổi bật cho nút duyệt
    
    btnLayout->addStretch(); // Đẩy nút sang bên phải hoặc căn giữa tùy ý
    btnLayout->addWidget(approveRequestBtn);
    layout->addLayout(btnLayout);

    // Gắn sự kiện vào hàm handleApproveRequest
    connect(approveRequestBtn, &QPushButton::clicked, this, &AdminPanel::handleApproveRequest);
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
        
        "QPushButton#safeBtn { background-color: #10b981; }" 
        "QPushButton#safeBtn:hover { background-color: #059669; }"
    );
}

void AdminPanel::loadUserData() {
    memberTable->setRowCount(0);
    QSqlQuery query("SELECT id, username, email, phone, role, status FROM users ORDER BY id ASC");
    int row = 0;
    while (query.next()) {
        memberTable->insertRow(row);
        for (int col = 0; col < 5; ++col) {
            memberTable->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
       
        QString status = query.value(5).toString().trimmed();
        if (status.isEmpty()) status = "Active";

        QTableWidgetItem *statusItem = new QTableWidgetItem(status == "Suspended" ? "Đã đình chỉ" : "Hoạt động");
        statusItem->setForeground(status == "Suspended" ? QBrush(Qt::red) : QBrush(Qt::darkGreen));
        memberTable->setItem(row, 5, statusItem);
        row++;
    }
}

void AdminPanel::viewSelectedUserDetail() {
    int currentRow = memberTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một độc giả trong bảng để xem chi tiết!");
        return;
    }

    QString id = memberTable->item(currentRow, 0)->text();
    QString username = memberTable->item(currentRow, 1)->text();
    QString email = memberTable->item(currentRow, 2)->text();
    QString phone = memberTable->item(currentRow, 3)->text();
    QString role = memberTable->item(currentRow, 4)->text();
    QString status = memberTable->item(currentRow, 5)->text();

    QDialog dlg(this);
    dlg.setWindowTitle("Chi tiết độc giả: " + username);
    dlg.setFixedSize(340, 270);
    dlg.setStyleSheet("background-color: white; color: #1e293b; font-size: 13px;");

    QVBoxLayout layout(&dlg);
    layout.setContentsMargins(20, 20, 20, 20);
    layout.setSpacing(10);

    QLabel *title = new QLabel("Thông Tin Chi Tiết Tài Khoản");
    title->setStyleSheet("font-size: 16px; font-weight: bold; color: #2563eb;");
    layout.addWidget(title);

    layout.addWidget(new QLabel("<b>Mã tài khoản (ID):</b> " + id));
    layout.addWidget(new QLabel("<b>Tên đăng nhập:</b> " + username));
    layout.addWidget(new QLabel("<b>Email:</b> " + email));
    layout.addWidget(new QLabel("<b>Số điện thoại:</b> " + phone));
    layout.addWidget(new QLabel("<b>Vai trò:</b> " + role));
    layout.addWidget(new QLabel("<b>Trạng thái:</b> " + status));

    layout.addStretch();
    QPushButton *closeBtn = new QPushButton("Đóng", &dlg);
    closeBtn->setStyleSheet("background-color: #2563eb; color: white; padding: 6px 14px; font-weight: bold; border-radius: 4px;");
    layout.addWidget(closeBtn);

    connect(closeBtn, &QPushButton::clicked, &dlg, &QDialog::accept);
    dlg.exec();
}

void AdminPanel::addNewMember() {
    QDialog dlg(this);
    dlg.setWindowTitle("Thêm thành viên mới");
    dlg.setFixedSize(350, 300);
    dlg.setStyleSheet("background-color: white; color: #1e293b; font-size: 13px;");

    QFormLayout form(&dlg);
    form.setSpacing(10);

    QLineEdit userEdit;
    QLineEdit passEdit;
    passEdit.setEchoMode(QLineEdit::Password);
    QLineEdit emailEdit;
    QLineEdit phoneEdit;

    form.addRow("Tên tài khoản:", &userEdit);
    form.addRow("Mật khẩu:", &passEdit);
    form.addRow("Email:", &emailEdit);
    form.addRow("Số điện thoại:", &phoneEdit);

    QHBoxLayout btnLayout;
    QPushButton saveBtn("Thêm");
    QPushButton cancelBtn("Hủy");
    saveBtn.setStyleSheet("background-color: #2563eb; color: white; padding: 6px 12px; font-weight: bold; border-radius: 4px;");
    cancelBtn.setStyleSheet("background-color: #64748b; color: white; padding: 6px 12px; font-weight: bold; border-radius: 4px;");
    
    btnLayout.addWidget(&saveBtn);
    btnLayout.addWidget(&cancelBtn);
    form.addRow(&btnLayout);

    connect(&cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(&saveBtn, &QPushButton::clicked, [&]() {
        QString username = userEdit.text().trimmed();
        QString password = passEdit.text().trimmed();

        if (username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(&dlg, "Lỗi", "Tên tài khoản và Mật khẩu không được để trống!");
            return;
        }

        QSqlQuery query;
        query.prepare("INSERT INTO users (username, password, email, phone, role, status) VALUES (:user, :pass, :email, :phone, 'Member', 'Active')");
        query.bindValue(":user", username);
        query.bindValue(":pass", password);
        query.bindValue(":email", emailEdit.text().trimmed());
        query.bindValue(":phone", phoneEdit.text().trimmed());

        if (query.exec()) {
            QMessageBox::information(&dlg, "Thành công", "Đã thêm thành viên mới thành công!");
            loadUserData();
            dlg.accept();
        } else {
            QMessageBox::critical(&dlg, "Lỗi", "Tên tài khoản có thể đã tồn tại trong hệ thống!");
        }
    });

    dlg.exec();
}

void AdminPanel::suspendSelectedUser() {
    int currentRow = memberTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một độc giả để đình chỉ!");
        return;
    }

    QString id = memberTable->item(currentRow, 0)->text();
    QString username = memberTable->item(currentRow, 1)->text();
    QString currentStatus = memberTable->item(currentRow, 5)->text();

    if (username.toLower() == "admin") {
        QMessageBox::warning(this, "Từ chối", "Không thể đình chỉ tài khoản quản trị viên gốc!");
        return;
    }
    
    if (currentStatus == "Đã đình chỉ") {
        QMessageBox::information(this, "Thông báo", "Tài khoản này đã bị đình chỉ từ trước!");
        return;
    }

    if (QMessageBox::question(this, "Xác nhận", "Bạn có chắc chắn muốn đình chỉ tài khoản: " + username + "?") == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("UPDATE users SET status = 'Suspended' WHERE id = :id");
        query.bindValue(":id", id);

        if (query.exec()) {
            QMessageBox::information(this, "Thành công", "Đã đình chỉ tài khoản thành công!");
            loadUserData();
        } else {
            QMessageBox::critical(this, "Lỗi", "Không thể cập nhật trạng thái đình chỉ vào cơ sở dữ liệu!");
        }
    }
}

void AdminPanel::unsuspendSelectedUser() {
    int currentRow = memberTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một độc giả để mở khóa (ngừng đình chỉ)!");
        return;
    }

    QString id = memberTable->item(currentRow, 0)->text();
    QString username = memberTable->item(currentRow, 1)->text();
    QString currentStatus = memberTable->item(currentRow, 5)->text();

    if (username.toLower() == "admin") {
        QMessageBox::information(this, "Thông báo", "Tài khoản quản trị viên gốc luôn ở trạng thái hoạt động!");
        return;
    }

    if (currentStatus == "Hoạt động") {
        QMessageBox::information(this, "Thông báo", "Tài khoản này đang hoạt động bình thường, không cần mở khóa!");
        return;
    }

    if (QMessageBox::question(this, "Xác nhận", "Bạn có chắc chắn muốn mở khóa cho tài khoản: " + username + "?") == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("UPDATE users SET status = 'Active' WHERE id = :id");
        query.bindValue(":id", id);

        if (query.exec()) {
            QMessageBox::information(this, "Thành công", "Đã mở khóa tài khoản thành công! Độc giả này có thể tiếp tục sử dụng hệ thống.");
            loadUserData();
        } else {
            QMessageBox::critical(this, "Lỗi", "Không thể cập nhật trạng thái mở khóa vào cơ sở dữ liệu!");
        }
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

    if (username.toLower() == "admin") {
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

// Cập nhật lại hàm duyệt: Đọc thẳng thông tin từ bảng thay vì truyền biến
void AdminPanel::handleApproveRequest() {
    int currentRow = requestTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một yêu cầu trong bảng để xử lý!");
        return;
    }

    QString requestId = requestTable->item(currentRow, 0)->text();
    QString userId = requestTable->item(currentRow, 1)->text();
    
    // Lấy Loại Yêu Cầu từ cột thứ 4 (Index = 3)
    QString actionType = requestTable->item(currentRow, 3)->text();
    
    QSqlQuery query;
    query.prepare("UPDATE requests SET status = 'Approved' WHERE id = :id");
    query.bindValue(":id", requestId);

    if (query.exec()) {
        // Gửi thông báo vào hộp thư của người dùng với Loại YC vừa lấy được
        QString message = QString("✅ Yêu cầu '%1' (Mã YC: %2) của bạn đã được Admin duyệt thành công!").arg(actionType, requestId);
        QString currentDate = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

        QSqlQuery notifQuery;
        notifQuery.prepare("INSERT INTO notifications (user_id, message, created_at) VALUES (:user_id, :message, :date)");
        notifQuery.bindValue(":user_id", userId);
        notifQuery.bindValue(":message", message);
        notifQuery.bindValue(":date", currentDate);
        notifQuery.exec();

        QMessageBox::information(this, "Thành công", "Đã duyệt yêu cầu thành công!");
        loadRequestData();
        emit dataChanged();
    } else {
        QMessageBox::critical(this, "Lỗi", "Không thể cập nhật trạng thái yêu cầu!");
    }
}

void AdminPanel::addBook() {
    QDialog dialog(this);
    dialog.setWindowTitle("Thêm sách mới");
    dialog.setFixedSize(360, 340);
    dialog.setStyleSheet("background-color: white; color: #1e293b;");

    QFormLayout form(&dialog);
    QLineEdit titleEdit, authorEdit, categoryEdit;
    QSpinBox yearSpin; yearSpin.setRange(1800, 2100); yearSpin.setValue(2026);
    QSpinBox qtySpin; qtySpin.setRange(0, 10000); qtySpin.setValue(1);
    QDoubleSpinBox priceSpin; priceSpin.setRange(0.0, 100000.0); priceSpin.setValue(10.0);

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
    QLineEdit titleEdit(currentTitle), authorEdit(currentAuthor), categoryEdit(currentCategory);
    QSpinBox yearSpin; yearSpin.setRange(1800, 2100); yearSpin.setValue(currentYear > 0 ? currentYear : 2026);
    QSpinBox qtySpin; qtySpin.setRange(0, 10000); qtySpin.setValue(currentQty);
    QDoubleSpinBox priceSpin; priceSpin.setRange(0.0, 100000.0); priceSpin.setValue(currentPrice);

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
        }
    }
}