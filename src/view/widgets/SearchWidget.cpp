#include "SearchWidget.h"
#include <QHBoxLayout>
#include <QLabel>

SearchWidget::SearchWidget(QWidget *parent) : QWidget(parent) {
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel("🔍 Thanh tìm kiếm (Search UI)"));
}