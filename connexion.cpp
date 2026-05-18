#include "connexion.h"
#include <QDebug>

Connexion::Connexion()
{
    // Initialize the QSqlDatabase object with the database connection
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("projet");
    db.setUserName("Ferieel");
    db.setPassword("Ferieel");
}

bool Connexion::create()
{
    // Open the database connection
    if (!db.open()) {
        qDebug() << "Error: Failed to connect database:" << db.lastError().text();
        return false;
    } else {
        qDebug() << "Database connected successfully";
        return true;
    }
}
