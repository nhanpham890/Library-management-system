#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <QWidget>

class AdminPanel : public QWidget {
    Q_OBJECT
public:
    explicit AdminPanel(QWidget *parent = nullptr);
};

#endif // ADMINPANEL_H