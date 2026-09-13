#include "FineListView.h"
#include <QVBoxLayout>
#include <QLabel>
FineListView::FineListView(QWidget *parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("⚠️ Danh sách tiền phạt (Fine list display)"));
}