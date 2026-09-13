#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QLineEdit>

class RegisterDialog : public QDialog {
    Q_OBJECT
public:
    explicit RegisterDialog(QWidget *parent = nullptr);

private:
    QLineEdit* userEdit;
    QLineEdit* passEdit;
    QLineEdit* emailEdit;
    QLineEdit* phoneEdit; // Thêm trường SĐT
};

#endif // REGISTERDIALOG_H