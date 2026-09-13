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
    
    // Quản lý Độc giả
    void viewSelectedUserDetail();
    void addNewMember();
    void suspendSelectedUser();
    void deleteSelectedUser();

    // Quản lý Sách
    void addBook();
    void editBook();
    void removeBook();
    
    // Quản lý Yêu cầu
    void handleApproveRequest(const QString &actionType);

private:
    QTabWidget *tabWidget;
    
    // Tab Độc giả
    QWidget *membersTab;
    QTableWidget *memberTable;
    QPushButton *refreshMemberBtn;
    QPushButton *viewMemberBtn;
    QPushButton *addMemberBtn;
    QPushButton *suspendMemberBtn;
    QPushButton *deleteMemberBtn;

    // Tab Sách
    QWidget *booksTab;
    QTableWidget *bookTable;
    QPushButton *addBookBtn;
    QPushButton *editBookBtn;
    QPushButton *removeBookBtn;

    // Tab Yêu cầu
    QWidget *requestsTab;
    QTableWidget *requestTable;
    QPushButton *approveBorrowBtn;
    QPushButton *approveReturnBtn;
    QPushButton *approveReserveBtn;
    QPushButton *approveRenewBtn;
};

#endif // ADMINPANEL_H