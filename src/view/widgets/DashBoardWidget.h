#ifndef DASHBOARDWIDGET_H
#define DASHBOARDWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>

class DashboardWidget : public QWidget {
    Q_OBJECT
public:
    explicit DashboardWidget(QWidget *parent = nullptr);

signals:
    void searchTriggered(const QString &criteria, const QString &keyword);

private:
    QLineEdit* searchBox;
    QComboBox* searchCriteriaBox; // Chọn tiêu chí: Title, Author, Subject, Publication Date
    QPushButton* searchBtn;
};

#endif // DASHBOARDWIDGET_H