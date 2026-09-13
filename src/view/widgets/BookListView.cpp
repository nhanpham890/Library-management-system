#include "BookListView.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QSqlQuery>

BookListView::BookListView(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; background: transparent; }");

    containerWidget = new QWidget();
    containerWidget->setStyleSheet("background: transparent;");
    gridLayout = new QGridLayout(containerWidget);
    gridLayout->setSpacing(16);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    scrollArea->setWidget(containerWidget);
    mainLayout->addWidget(scrollArea);

    loadBooksFromDatabase();
}

void BookListView::loadBooksFromDatabase(const QString &keyword, const QString &searchType) {
    QLayoutItem *child;
    while ((child = gridLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
    }

    QSqlQuery query;
    QString sql = "SELECT id, title, author, category, year, quantity, price FROM books";
    
    if (!keyword.isEmpty()) {
        if (searchType == "Search by title") {
            sql += " WHERE title LIKE :keyword";
        } else if (searchType == "Search by author") {
            sql += " WHERE author LIKE :keyword";
        } else if (searchType == "Search by subject") {
            sql += " WHERE category LIKE :keyword";
        } else if (searchType == "Search by publication date") {
            sql += " WHERE year LIKE :keyword";
        } else {
            sql += " WHERE title LIKE :keyword OR author LIKE :keyword OR category LIKE :keyword OR year LIKE :keyword";
        }
    }
    
    sql += " ORDER BY id ASC";
    query.prepare(sql);
    
    if (!keyword.isEmpty()) {
        query.bindValue(":keyword", "%" + keyword + "%");
    }
    
    query.exec();

    int index = 0;
    while (query.next()) {
        int id = query.value(0).toInt();
        QString title = query.value(1).toString();
        QString author = query.value(2).toString();
        QString category = query.value(3).toString();
        int year = query.value(4).toInt();
        int quantity = query.value(5).toInt();
        double price = query.value(6).toDouble();

        QPushButton *card = new QPushButton(containerWidget);
        card->setCursor(Qt::PointingHandCursor);
        card->setFixedSize(220, 220);
        card->setStyleSheet(
            "QPushButton {"
            "   background-color: white;"
            "   border: 1px solid #e2e8f0;"
            "   border-radius: 8px;"
            "   text-align: left;"
            "}"
            "QPushButton:hover {"
            "   border: 1px solid #3b82f6;"
            "   background-color: #f8fafc;"
            "}"
        );

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(12, 12, 12, 12);
        cardLayout->setSpacing(6);

        QLabel *coverImg = new QLabel();
        coverImg->setFixedHeight(110);
        coverImg->setText("Bìa Sách");
        coverImg->setAlignment(Qt::AlignCenter);
        coverImg->setStyleSheet("background-color: #f1f5f9; border-radius: 4px; color: #64748b; font-size: 13px;");
        cardLayout->addWidget(coverImg);

        QLabel *titleLabel = new QLabel(title);
        titleLabel->setWordWrap(true);
        titleLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #0f172a; border: none; background: transparent;");
        cardLayout->addWidget(titleLabel);

        QLabel *authorLabel = new QLabel("Tác giả: " + author);
        authorLabel->setStyleSheet("font-size: 12px; color: #64748b; border: none; background: transparent;");
        cardLayout->addWidget(authorLabel);

        QLabel *catLabel = new QLabel(category);
        catLabel->setStyleSheet("background-color: #fed7aa; color: #9a3412; font-size: 11px; padding: 2px 6px; border-radius: 4px; border: none; max-width: 160px;");
        cardLayout->addWidget(catLabel);
        cardLayout->addStretch();

        connect(card, &QPushButton::clicked, this, [=]() {
            QSqlQuery freshQuery;
            freshQuery.prepare("SELECT quantity FROM books WHERE id = ?");
            freshQuery.addBindValue(id);
            int currentQty = quantity;
            if (freshQuery.exec() && freshQuery.next()) {
                currentQty = freshQuery.value(0).toInt();
            }

            showBookDetailDialog(id, title, author, category, year, currentQty, price);
        });

        int row = index / 4;
        int col = index % 4;
        gridLayout->addWidget(card, row, col);
        index++;
    }
}

void BookListView::showBookDetailDialog(int id, const QString &title, const QString &author, const QString &category, int year, int quantity, double price) {
    QDialog detailDialog(this);
    detailDialog.setWindowTitle("Chi tiết sách: " + title);
    detailDialog.setFixedSize(380, 420);
    detailDialog.setStyleSheet("background-color: white; color: #1e293b;");

    QVBoxLayout *layout = new QVBoxLayout(&detailDialog);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(12);

    QLabel *cover = new QLabel();
    cover->setFixedHeight(140);
    cover->setText("Bìa Sách");
    cover->setAlignment(Qt::AlignCenter);
    cover->setStyleSheet("background-color: #f1f5f9; border-radius: 6px; color: #64748b; font-size: 16px;");
    layout->addWidget(cover);

    QLabel *lblTitle = new QLabel(title);
    lblTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #0f172a;");
    layout->addWidget(lblTitle);

    QLabel *lblAuthor = new QLabel("Tác giả: " + author);
    lblAuthor->setStyleSheet("font-size: 13px; color: #475569;");
    layout->addWidget(lblAuthor);

    QLabel *lblCat = new QLabel("Thể loại: " + category);
    lblCat->setStyleSheet("font-size: 13px; color: #475569;");
    layout->addWidget(lblCat);

    QLabel *lblYear = new QLabel("Năm xuất bản: " + QString::number(year));
    lblYear->setStyleSheet("font-size: 13px; color: #475569;");
    layout->addWidget(lblYear);

    QLabel *lblQty = new QLabel("Số lượng kho: " + QString::number(quantity) + " quyển");
    lblQty->setStyleSheet("font-size: 13px; color: #16a34a; font-weight: bold;");
    layout->addWidget(lblQty);

    QLabel *lblPrice = new QLabel("Giá sách: $" + QString::number(price, 'f', 2));
    lblPrice->setStyleSheet("font-size: 14px; color: #dc2626; font-weight: bold;");
    layout->addWidget(lblPrice);

    layout->addStretch();

    QPushButton *closeBtn = new QPushButton("Đóng");
    closeBtn->setCursor(Qt::PointingHandCursor);
    closeBtn->setStyleSheet("background-color: #2563eb; color: white; padding: 9px; border-radius: 5px; font-weight: bold;");
    layout->addWidget(closeBtn);

    connect(closeBtn, &QPushButton::clicked, &detailDialog, &QDialog::accept);

    detailDialog.exec();
}