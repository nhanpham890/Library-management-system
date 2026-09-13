// library.cpp : Defines the entry point for the application.
//

#include "library.h"

using namespace std;
#include "database.h"

Database::Database() {
    db = QSqlDatabase::addDatabase("QSQLITE");
}

Database& Database::getInstance() {
    static Database instance;
    return instance;
}

bool Database::connectDB(const QString& dbPath) {
    db.setDatabaseName(dbPath);
    if (!db.open()) {
        qDebug() << "Error: Failed to connect database." << db.lastError().text();
        return false;
    }
    qDebug() << "Database connected successfully!";
    initTables();
    return true;
}

void Database::initTables() {
    QSqlQuery query;

    // Bảng Sách
    query.exec("CREATE TABLE IF NOT EXISTS Books ("
        "id TEXT PRIMARY KEY, "
        "title TEXT, "
        "author TEXT, "
        "category TEXT, "
        "quantity INTEGER)");

    // Bảng Độc giả / Thành viên
    query.exec("CREATE TABLE IF NOT EXISTS Members ("
        "memberId TEXT PRIMARY KEY, "
        "name TEXT, "
        "email TEXT)");

    // Bảng Người dùng hệ thống
    query.exec("CREATE TABLE IF NOT EXISTS Users ("
        "userId TEXT PRIMARY KEY, "
        "username TEXT, "
        "password TEXT, "
        "role TEXT)");

    // Bảng Phiếu mượn/trả
    query.exec("CREATE TABLE IF NOT EXISTS Requests ("
        "requestId TEXT PRIMARY KEY, "
        "userId TEXT, "
        "bookId TEXT, "
        "borrowDate TEXT, "
        "dueDate TEXT, "
        "status TEXT)");

    // Bảng Tiền phạt
    query.exec("CREATE TABLE IF NOT EXISTS Fines ("
        "fineId TEXT PRIMARY KEY, "
        "requestId TEXT, "
        "amount REAL, "
        "status TEXT)");
}

QSqlDatabase& Database::getDatabase() {
    return db;
}

Database::~Database() {
    if (db.isOpen()) {
        db.close();
    }
}
