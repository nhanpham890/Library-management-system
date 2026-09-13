#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

class Database {
public:
    static Database& instance() {
        static Database dbInstance;
        return dbInstance;
    }

    bool init() {
        if (QSqlDatabase::contains("qt_sql_default_connection")) {
            db = QSqlDatabase::database("qt_sql_default_connection");
        } else {
            db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName("library.db");
        }

        if (!db.open()) {
            return false;
        }

        QSqlQuery query;

        query.exec("CREATE TABLE IF NOT EXISTS users ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "username TEXT UNIQUE NOT NULL, "
                   "password TEXT NOT NULL, "
                   "email TEXT, "
                   "phone TEXT, "
                   "role TEXT DEFAULT 'Member')");

        query.exec("CREATE TABLE IF NOT EXISTS books ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "title TEXT NOT NULL, "
                   "author TEXT, "
                   "category TEXT, "
                   "year INTEGER DEFAULT 2024, "
                   "quantity INTEGER DEFAULT 0, "
                   "price REAL DEFAULT 0.0)");

        query.exec("CREATE TABLE IF NOT EXISTS requests ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "user_id INTEGER, "
                   "book_id INTEGER, "
                   "type TEXT, "
                   "status TEXT, "
                   "request_date DATE)");

        QSqlQuery checkAdmin;
        checkAdmin.exec("SELECT id FROM users WHERE username = 'admin'");
        if (!checkAdmin.next()) {
            QSqlQuery insertAdmin;
            insertAdmin.prepare("INSERT INTO users (username, password, email, phone, role) "
                                "VALUES ('admin', 'admin123', 'admin@uth.edu.vn', '0123456789', 'Admin')");
            insertAdmin.exec();
        }

        QSqlQuery checkBooks;
        checkBooks.exec("SELECT COUNT(*) FROM books");
        if (checkBooks.next() && checkBooks.value(0).toInt() == 0) {
            query.exec("INSERT INTO books (title, author, category, year, quantity, price) VALUES "
                       "('The Catcher in the Rye', 'J.D. Salinger', 'Anthologies & Collections', 1951, 6, 8.99), "
                       "('Atomic Habits', 'James Clear', 'Self-Help', 2018, 6, 11.99), "
                       "('The Lean Startup', 'Eric Ries', 'Business & Economics', 2011, 7, 16.00), "
                       "('Educated: A Memoir', 'Tara Westover', 'Academic/Reference', 2018, 12, 14.50), "
                       "('The Subtle Art', 'Mark Manson', 'Business & Economics', 2016, 3, 12.99)");
        }

        return true;
    }

    QSqlDatabase& getDb() {
        return db;
    }

private:
    Database() {}
    ~Database() {
        if (db.isOpen()) db.close();
    }
    QSqlDatabase db;
};

#endif