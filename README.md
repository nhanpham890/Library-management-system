# Library-management-system
project-software-engineer
* **Ngôn ngữ**: C++ (C++11 trở lên)
* **GUI Framework**: Qt (QtWidgets, QCoreApplication, QProcess)
* **Cơ sở dữ liệu**: SQLite (`QSql`, `QSqlQuery`)
* **Build System**: CMake / MinGW

* LibraryManagementSystem/
│
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── models/
│   │   └── Database.h / .cpp
│   ├── view/
│   │   ├── MainWindow.h / .cpp
│   │   ├── widgets/
│   │   │   ├── AdminPanel.h / .cpp
│   │   │   ├── BookListView.h / .cpp
│   │   │   ├── DashboardWidget.h / .cpp
│   │   │   └── StatusBar.h / .cpp
│   │   └── dialogs/
│   │       ├── LoginDialog.h / .cpp
│   │       ├── RegisterDialog.h / .cpp
│   │       ├── UserDetailDialog.h / .cpp
│   │       └── RequestDialog.h / .cpp
