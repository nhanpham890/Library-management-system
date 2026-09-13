cmake_minimum_required(VERSION 3.16)
project(QtFirstApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Chỉ khai báo các file thực sự tồn tại trong cây thư mục hiện tại
add_executable(QtFirstApp 
    src/main.cpp 
    src/view/MainWindow.cpp
    src/view/dialogs/AdminDialog.cpp
    src/view/dialogs/BorrowDialog.cpp
    src/view/dialogs/LoginDialog.cpp
    src/view/dialogs/RegisterDialog.cpp
    src/view/widgets/AdminPanel.cpp
    src/view/widgets/BookListView.cpp
    src/view/widgets/BorrowView.cpp
    src/view/widgets/DashboardWidget.cpp
    src/view/widgets/FineListView.cpp
    src/view/widgets/SearchWidget.cpp
    src/view/widgets/StatusBar.cpp
)

target_include_directories(QtFirstApp PRIVATE 
    src
    src/view
    src/view/dialogs
    src/view/widgets
)

target_link_libraries(QtFirstApp PRIVATE Qt6::Widgets)