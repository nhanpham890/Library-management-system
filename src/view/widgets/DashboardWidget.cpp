#include "DashboardWidget.h"
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

DashboardWidget::DashboardWidget(QWidget *parent) : QWidget(parent) {
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 5, 10, 5);

    // 1. TẠO VÀ THIẾT LẬP Ô CHỌN TRƯỚC
    searchCriteriaBox = new QComboBox();
    searchCriteriaBox->addItem("Search catalog (Tất cả)");
    searchCriteriaBox->addItem("Search by title");
    searchCriteriaBox->addItem("Search by author");
    searchCriteriaBox->addItem("Search by subject");
    searchCriteriaBox->addItem("Search by publication date");
    
    searchCriteriaBox->setStyleSheet(
        "QComboBox {"
        "   padding: 8px;"
        "   border: 1px solid #ccc;"
        "   border-radius: 6px;"
        "   font-size: 13px;"
        "   background-color: #ffffff;"
        "   color: #000000;"
        "}"
        "QComboBox QAbstractItemView {"
        "   background-color: #ffffff;"
        "   color: #000000;"
        "   selection-background-color: #0052cc;"
        "   selection-color: #ffffff;"
        "}"
    );

    // 2. TẠO VÀ THIẾT LẬP Ô NHẬP TỪ KHÓA
    searchBox = new QLineEdit();
    searchBox->setPlaceholderText("Nhập thông tin cần tìm kiếm...");
    searchBox->setStyleSheet(
        "QLineEdit {"
        "   padding: 8px 12px;"
        "   border: 1px solid #ccc;"
        "   border-radius: 6px;"
        "   font-size: 13px;"
        "   background-color: #ffffff;"
        "   color: #000000;"
        "}"
    );

    // 3. TẠO NÚT TÌM KIẾM
    searchBtn = new QPushButton("Tìm kiếm");
    searchBtn->setCursor(Qt::PointingHandCursor);
    searchBtn->setStyleSheet("padding: 8px 16px; background-color: #0052cc; color: white; border-radius: 6px; font-weight: bold;");

    // 4. THÊM TẤT CẢ VÀO LAYOUT (Chỉ được thêm khi đã khởi tạo xong 3 món ở trên)
    layout->addWidget(searchCriteriaBox, 2);
    layout->addWidget(searchBox, 3);
    layout->addWidget(searchBtn, 1);

    // 5. KẾT NỐI SỰ KIỆN XỬ LÝ (Luôn nằm ở cuối cùng)
    connect(searchBtn, &QPushButton::clicked, this, [this]() {
        emit searchTriggered(searchCriteriaBox->currentText(), searchBox->text());
    });
    
    connect(searchBox, &QLineEdit::returnPressed, this, [this]() {
        emit searchTriggered(searchCriteriaBox->currentText(), searchBox->text());
    });
}