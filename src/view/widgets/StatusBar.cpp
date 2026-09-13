#include "StatusBar.h"
#include <QHBoxLayout>

StatusBar::StatusBar(QWidget *parent) : QWidget(parent) {
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 5, 10, 5);

    statusLabel = new QLabel("Hệ thống sẵn sàng", this);
    statusLabel->setStyleSheet("color: #94a3b8; font-size: 12px; background: transparent;");
    
    layout->addWidget(statusLabel);
}

void StatusBar::setStatusMessage(const QString &message) {
    statusLabel->setText(message);
}