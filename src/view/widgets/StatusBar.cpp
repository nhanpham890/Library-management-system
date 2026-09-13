#include "StatusBar.h"
#include <QHBoxLayout>
#include <QLabel>
StatusBar::StatusBar(QWidget *parent) : QWidget(parent) {
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel("✅ Ready - Trạng thái hệ thống"));
}