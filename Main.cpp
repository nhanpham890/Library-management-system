#include <QApplication>
#include <QDebug>
#include "models/Database.h"
#include "controllers/AuthController.h"
#include "controllers/BookController.h"
#include "views/LoginWindow.h" // Khởi chạy màn hình đăng nhập đầu tiên

int main(int argc, char *argv[]) {
    // 1. Khởi tạo ứng dụng Qt (Bắt buộc cho mọi ứng dụng giao diện Desktop)
    QApplication app(argc, argv);

    // 2. Khởi tạo và kết nối Cơ sở dữ liệu
    Database db;
    if (!db.connect()) {
        qDebug() << "Lỗi: Không thể kết nối tới cơ sở dữ liệu!";
        return -1; // Thoát chương trình nếu lỗi kết nối CSDL
    }
    qDebug() << "Kết nối cơ sở dữ liệu thành công!";

    // 3. Khởi tạo các Controller chứa logic nghiệp vụ và truyền Database vào
    AuthController authController(&db);
    BookController bookController(&db);

    // 4. Hiển thị màn hình đăng nhập (LoginWindow) làm giao diện mở đầu
    // Truyền authController sang giao diện để xử lý sự kiện nút Đăng nhập / Đăng ký
    LoginWindow loginWindow(&authController);
    loginWindow.show();

    // 5. Chạy vòng lặp sự kiện chính của ứng dụng
    return app.exec();
}