#include <QApplication>
#include "view/MainWindow.h"
#include "models/Database.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Database::instance().init();

    MainWindow window;
    window.show();

    return app.exec();
}