// Kế thừa QWidget
#include "AdminPanel.h"
#include <QVBoxLayout>
#include <QLabel>
AdminPanel::AdminPanel(QWidget *parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("⚙️ Bảng điều khiển Quản trị (Admin UI)"));
}