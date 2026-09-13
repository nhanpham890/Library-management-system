#ifndef USERDETAILDIALOG_H
#define USERDETAILDIALOG_H

#include <QDialog>
#include <QString>

class QLabel;

class UserDetailDialog : public QDialog {
    Q_OBJECT
public:
    explicit UserDetailDialog(const QString &username, QWidget *parent = nullptr);

private:
    void loadUserInfo(const QString &username);

    QLabel *lblUsername;
    QLabel *lblEmail;
    QLabel *lblPhone;
    QLabel *lblRole;
};

#endif