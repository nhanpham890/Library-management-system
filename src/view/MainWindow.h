#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QScrollArea>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void handleLoginLogout();

private:
    void setupUI();
    void loadBooks();

    bool isLoggedIn;
    QLabel* userLabel;
    QPushButton* loginBtn;
    QGridLayout* gridLayout;
};

#endif // MAINWINDOW_H