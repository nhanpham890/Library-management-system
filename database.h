#pragma once
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QDebug>

class Database {
private:
    QSqlDatabase db;
    Database();

public:
    static Database& getInstance();
    bool connectDB(const QString& dbPath = "library.db");
    void initTables();
    QSqlDatabase& getDatabase();
    ~Database();
};
