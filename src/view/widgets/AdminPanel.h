#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>

class AdminPanel : public QWidget {
    Q_OBJECT
public:
    explicit AdminPanel(QWidget *parent = nullptr);

signals:
    void dataChanged();

private:
    void setupMembersTab();
    void setupBooksTab();
    void setupRequestsTab();
    void setupStyles();
    void loadUserData();
    void loadBookData();
    void loadRequestData();
    
    void addBook();
    void editBook();
    void removeBook();
    
    // Khai báo hàm xử lý duyệt yêu cầu để sửa lỗi biên dịch
    void handleApproveRequest(const QString &actionType);

private slots:
    void deleteSelectedUser();

private:
    QTabWidget *tabWidget;
    
    // Quản lý Độc giả
    QWidget *membersTab;
    QTableWidget *memberTable;
    QPushButton *refreshMemberBtn;
    QPushButton *viewMemberBtn;
    QPushButton *addMemberBtn;
    QPushButton *suspendMemberBtn;
    QPushButton *deleteMemberBtn;

    // Quản lý Sách
    QWidget *booksTab;
    QTableWidget *bookTable;
    QPushButton *addBookBtn;
    QPushButton *editBookBtn;
    QPushButton *removeBookBtn;

    // Quản lý Yêu cầu
    QWidget *requestsTab;
    QTableWidget *requestTable;
    QPushButton *approveBorrowBtn;
    QPushButton *approveReturnBtn;
    QPushButton *approveReserveBtn;
    QPushButton *approveRenewBtn;
};

#endif // ADMINPANEL_H