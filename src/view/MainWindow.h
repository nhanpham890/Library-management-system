#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widgets/SearchWidget.h"
#include "widgets/BookListView.h"
#include "widgets/DashboardWidget.h"
#include "widgets/StatusBar.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void handleLoginAction();

private:
    SearchWidget* searchWidget;
    BookListView* bookListView;
    DashboardWidget* dashboardWidget;
    StatusBar* statusBarWidget;
    bool isLoggedIn;
};

#endif // MAINWINDOW_H