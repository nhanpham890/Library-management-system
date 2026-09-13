#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QPushButton;
class DashboardWidget;
class BookListView;
class StatusBar;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private:
    bool isLoggedIn;
    QPushButton *userStatusBtn;
    QPushButton *authBtn;
    DashboardWidget *dashboardWidget;
    BookListView *bookListView;
    StatusBar *statusBarWidget;
};

#endif