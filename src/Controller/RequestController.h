#ifndef REQUEST_CONTROLLER_H
#define REQUEST_CONTROLLER_H

#include <QString>
#include <QList>
#include "../models/Database.h"
#include "../models/Request.h" // Class thực thể phiếu mượn/trả

class RequestController {
private:
    Database* db; // Con trỏ kết nối cơ sở dữ liệu

public:
    RequestController(Database* database);

    // Người dùng gửi yêu cầu mượn sách
    bool createBorrowRequest(int userId, int bookId);

    // Admin duyệt yêu cầu mượn sách
    bool approveRequest(int requestId);

    // Xử lý trả sách (cập nhật trạng thái và ngày trả)
    bool returnBook(int requestId, int bookId);

    // Gia hạn thời gian mượn sách
    bool extendRequest(int requestId);

    // Lấy danh sách các phiếu mượn (để hiển thị lên giao diện)
    QList<Request> getAllRequests();
};

#endif // REQUEST_CONTROLLER_H