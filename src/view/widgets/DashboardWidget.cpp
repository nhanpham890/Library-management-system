#include "DashboardWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

DashboardWidget::DashboardWidget(QWidget *parent) : QWidget(parent) {
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    searchCriteriaBox = new QComboBox(this);
    searchCriteriaBox->addItem("Search by title");
    searchCriteriaBox->addItem("Search by author");
    searchCriteriaBox->addItem("Search by subject");
    searchCriteriaBox->addItem("Search by publication date");
    searchCriteriaBox->setFixedWidth(200);
    searchCriteriaBox->setStyleSheet("padding: 8px; border: 1px solid #cbd5e1; border-radius: 4px; background: white; color: black;");

    searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("Nhập từ khóa tìm kiếm...");
    searchBox->setStyleSheet("padding: 8px; border: 1px solid #cbd5e1; border-radius: 4px; background: white; color: black;");

    searchBtn = new QPushButton("Tìm kiếm", this);
    searchBtn->setCursor(Qt::PointingHandCursor);
    searchBtn->setStyleSheet("background-color: #2563eb; color: white; padding: 8px 16px; border-radius: 4px; font-weight: bold;");

    layout->addWidget(searchCriteriaBox);
    layout->addWidget(searchBox);
    layout->addWidget(searchBtn);

    auto triggerSearch = [=]() {
        emit searchTriggered(searchCriteriaBox->currentText(), searchBox->text().trimmed());
    };

    connect(searchBtn, &QPushButton::clicked, this, triggerSearch);
    connect(searchBox, &QLineEdit::returnPressed, this, triggerSearch);
}