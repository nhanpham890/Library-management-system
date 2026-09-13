#ifndef REQUESTDIALOG_H
#define REQUESTDIALOG_H

#include <QDialog>

class QComboBox;
class QLineEdit;
class QTextEdit;

class RequestDialog : public QDialog {
    Q_OBJECT
public:
    explicit RequestDialog(const QString &username, QWidget *parent = nullptr);

private slots:
    void handleSubmit();

private:
    void loadUserBooks();

    QString currentUsername;
    QComboBox *requestTypeCombo;
    QComboBox *bookCombo;
    QTextEdit *noteEdit;
};

#endif // REQUESTDIALOG_H