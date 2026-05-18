#ifndef CONNEXION_H
#define CONNEXION_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QString>



class Connexion
{
    QSqlDatabase db;
public :
    Connexion();
    bool create ();
};

#endif // CONNEXION_H
