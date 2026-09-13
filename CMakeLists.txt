cmake_minimum_required(VERSION 3.16)

project(QtFirstApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

add_executable(QtFirstApp
    src/main.cpp
    src/models/Database.h

    src/controller/AdminController.h
    src/controller/AdminController.cpp
    src/controller/BookController.h
    src/controller/BookController.cpp
    src/controller/RequestController.h
    src/controller/RequestController.cpp
    src/controller/LoginController.h
    src/controller/LoginController.cpp

    src/view/MainWindow.h
    src/view/MainWindow.cpp

    src/view/dialogs/RequestDialog.h
    src/view/dialogs/RequestDialog.cpp
    src/view/dialogs/AdminDialog.h
    src/view/dialogs/AdminDialog.cpp
    src/view/dialogs/BorrowDialog.h
    src/view/dialogs/BorrowDialog.cpp
    src/view/dialogs/LoginDialog.h
    src/view/dialogs/LoginDialog.cpp
    src/view/dialogs/RegisterDialog.h
    src/view/dialogs/RegisterDialog.cpp
    src/view/dialogs/UserDetailDialog.cpp
    src/view/dialogs/UserDetailDialog.cpp
    src/view/dialogs/MailBoxDialog.h
    src/view/dialogs/MailBoxDialog.cpp

    src/view/widgets/AdminPanel.h
    src/view/widgets/AdminPanel.cpp
    src/view/widgets/BookListView.h
    src/view/widgets/BookListView.cpp
    src/view/widgets/BorrowView.h
    src/view/widgets/BorrowView.cpp
    src/view/widgets/DashboardWidget.h
    src/view/widgets/DashboardWidget.cpp
    src/view/widgets/FineListView.h
    src/view/widgets/FineListView.cpp
    src/view/widgets/SearchWidget.h
    src/view/widgets/SearchWidget.cpp
    src/view/widgets/StatusBar.h
    src/view/widgets/StatusBar.cpp
)

target_include_directories(QtFirstApp PRIVATE
    src
    src/controller
    src/models
    src/view
    src/view/dialogs
    src/view/widgets
)

target_link_libraries(QtFirstApp PRIVATE
    Qt6::Widgets
    Qt6::Sql
)