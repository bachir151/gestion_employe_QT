#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QSqlDatabase>
#include <QSqlQueryModel>

class DatabaseConnection
{
public:
    DatabaseConnection();
    bool testConnection();
    QSqlQueryModel* afficheEmploye();
    void ajouteEmploye(QString id, QString nom, QString prenom, QString email, QString numero);
    QSqlQueryModel* rechercheEmploye(QMap<QString, QString>);
    void modifieEmploye(QString id, QString nom, QString prenom, QString email, QString numero);
    void supprimeEmploye(QString id);


private :
    QSqlDatabase mDatabase;
};

#endif // DATABASECONNECTION_H
