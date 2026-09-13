#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class SearchWidget : public QWidget {
    Q_OBJECT
public:
    explicit SearchWidget(QWidget *parent = nullptr);
    void updateUserInfo(const QString& username, bool loggedIn);

signals:
    void loginButtonClicked();

private:
    QLabel* userLabel;
    QLabel* logoLabel;
    QPushButton* loginBtn;
    bool isLoggedIn;
};

#endif // SEARCHWIDGET_H