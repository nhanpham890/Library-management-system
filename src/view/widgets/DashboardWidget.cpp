#include "DashboardWidget.h"
#include <QVBoxLayout>
#include <QLabel>
DashboardWidget::DashboardWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("📊 Bảng điều khiển thành viên (Member dashboard)"));
}