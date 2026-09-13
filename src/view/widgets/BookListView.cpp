#include "BookListView.h"
#include <QVBoxLayout>
#include <QLabel>

QFrame* createInlineBookCard(const QString& title, const QString& author, const QString& subject, const QString& pubDate, const QString& quantity, const QString& price, const QString& catColor) {
    QFrame* card = new QFrame();
    card->setStyleSheet("QFrame { background-color: white; border: 1px solid #e0e0e0; border-radius: 8px; } QLabel { border: none; }");
    card->setFixedSize(210, 310);

    QVBoxLayout* layout = new QVBoxLayout(card);
    layout->setSpacing(5);
    layout->setContentsMargins(12, 12, 12, 12);

    QLabel* imgLabel = new QLabel("Bìa Sách");
    imgLabel->setAlignment(Qt::AlignCenter);
    imgLabel->setStyleSheet("background-color: #f0f0f0; color: #888; border-radius: 4px; font-size: 12px;");
    imgLabel->setFixedHeight(90);
    layout->addWidget(imgLabel);

    QLabel* titleLabel = new QLabel("<b>" + title + "</b>");
    titleLabel->setWordWrap(true);
    titleLabel->setStyleSheet("font-size: 13px; color: #222;");
    titleLabel->setFixedHeight(35);
    layout->addWidget(titleLabel);

    QLabel* authorLabel = new QLabel("Tác giả: " + author);
    authorLabel->setStyleSheet("font-size: 11px; color: #555;");
    layout->addWidget(authorLabel);

    QLabel* subLabel = new QLabel("Chủ đề: " + subject);
    subLabel->setStyleSheet("background-color: " + catColor + "; padding: 2px 4px; border-radius: 3px; font-size: 10px; color: #333;");
    subLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    layout->addWidget(subLabel);

    QLabel* dateLabel = new QLabel("Năm XB: " + pubDate);
    dateLabel->setStyleSheet("font-size: 11px; color: #666;");
    layout->addWidget(dateLabel);

    QLabel* qtyLabel = new QLabel("Kho: " + quantity + " | 🟢 Có sẵn");
    qtyLabel->setStyleSheet("font-size: 11px; color: #2e7d32; font-weight: bold;");
    layout->addWidget(qtyLabel);

    QLabel* priceLabel = new QLabel("Giá: " + price);
    priceLabel->setStyleSheet("font-size: 12px; color: #444; font-weight: bold;");
    layout->addWidget(priceLabel);

    layout->addStretch();
    return card;
}

BookListView::BookListView(QWidget *parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; background-color: #f5f7f9; }");

    QWidget* scrollContent = new QWidget();
    scrollContent->setStyleSheet("background-color: #f5f7f9;");
    gridLayout = new QGridLayout(scrollContent);
    gridLayout->setSpacing(20);
    gridLayout->setContentsMargins(20, 20, 20, 20);

    // Khởi tạo danh sách sách mẫu chuẩn theo các tiêu chí trong Use Case
    allBooks = {
        {"The Catcher in the Rye", "J.D. Salinger", "Anthologies & Collections", "1951", "6", "8.99", "#e0e0e0", nullptr},
        {"Atomic Habits", "James Clear", "Non-Fiction", "2018", "6", "11.98", "#ffe0b2", nullptr},
        {"Harry Potter", "J.K. Rowling", "Poetry and Drama", "1997", "2", "9.99", "#e1bee7", nullptr},
        {"The Lean Startup", "Eric Ries", "Business & Economics", "2011", "7", "16.00", "#ffcdd2", nullptr},
        {"Educated: A Memoir", "Tara Westover", "Academic/Reference", "2018", "12", "9.00", "#b3e5fc", nullptr},
        {"The Subtle Art", "Mark Manson", "Business & Economics", "2016", "3", "12.99", "#ffcdd2", nullptr}
    };

    for(int i = 0; i < allBooks.size(); ++i) {
        int row = i / 4; 
        int col = i % 4; 
        allBooks[i].cardWidget = createInlineBookCard(
            allBooks[i].title, allBooks[i].author, allBooks[i].subject, 
            allBooks[i].pubDate, allBooks[i].qty, allBooks[i].price, allBooks[i].color
        );
        gridLayout->addWidget(allBooks[i].cardWidget, row, col);
    }

    gridLayout->setRowStretch(gridLayout->rowCount(), 1);
    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);
}

// Xử lý logic tìm kiếm phân rã theo biểu đồ Use Case (Search catalog -> Search by...)
void BookListView::searchBooks(const QString &criteria, const QString &keyword) {
    int visibleIndex = 0;
    QString query = keyword.trimmed().toLower();

    for (auto &book : allBooks) {
        bool match = false;

        if (criteria.contains("title")) {
            match = book.title.toLower().contains(query);
        } else if (criteria.contains("author")) {
            match = book.author.toLower().contains(query);
        } else if (criteria.contains("subject")) {
            match = book.subject.toLower().contains(query);
        } else if (criteria.contains("publication date")) {
            match = book.pubDate.toLower().contains(query);
        } else {
            // Tìm kiếm tổng quát trên tất cả trường nếu chọn mục "Tất cả"
            match = book.title.toLower().contains(query) || 
                    book.author.toLower().contains(query) || 
                    book.subject.toLower().contains(query) || 
                    book.pubDate.toLower().contains(query);
        }

        if (query.isEmpty() || match) {
            int row = visibleIndex / 4;
            int col = visibleIndex % 4;
            gridLayout->addWidget(book.cardWidget, row, col);
            book.cardWidget->show();
            visibleIndex++;
        } else {
            book.cardWidget->hide();
        }
    }
}