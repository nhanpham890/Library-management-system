#include "AdminDialog.h"
#include <QVBoxLayout>
#include <QLabel>

AdminDialog::AdminDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Thao tác Admin");
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Giao diện cấu hình Admin ở đây"));
}