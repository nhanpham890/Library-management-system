#ifndef MAILBOXDIALOG_H
#define MAILBOXDIALOG_H

#include <QDialog>
#include <QListWidget>

class MailboxDialog : public QDialog {
    Q_OBJECT
public:
    explicit MailboxDialog(const QString &username, QWidget *parent = nullptr);

private:
    void loadMessages();
    QString currentUsername;
    QListWidget *listWidget;
};

#endif // MAILBOXDIALOG_H