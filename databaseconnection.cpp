#include "databaseconnection.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>

DatabaseConnection::DatabaseConnection() {
    mDatabase = QSqlDatabase::addDatabase("QODBC");
    mDatabase.setDatabaseName("ODBCPP");
}


bool DatabaseConnection::testConnection(){

    if (!mDatabase.open()){
        qDebug() << "Echec ouverture base de données";
        return true;
    } else {
        qDebug() << "Base de données ouverte avec succès";
        return false;
    }
}

QSqlQueryModel* DatabaseConnection::afficheEmploye(){
    QString requete = "SELECT TOP 10 * FROM employe";
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query(mDatabase);
    query.prepare(requete);

    if (!query.exec(requete)){
        QMessageBox::critical(nullptr,"Echec requête", query.lastError().text());
        return model;
    } else {
        QMessageBox::information(nullptr, "Succès","Requête exécutée avec succès");
    }

    model->setQuery(std::move(query));
    return model;

}

void DatabaseConnection::ajouteEmploye(QString id, QString nom, QString prenom, QString email, QString numero){
    QSqlQuery query(mDatabase);
    QString requete = "insert into employe (id, nom, prenom, email, numero) values (:id, :nom, :prenom, :email, :numero)";
    query.prepare(requete);
    query.bindValue(":id",id);
    query.bindValue(":nom",nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":numero", numero);

    if (!query.exec()){
        QMessageBox::critical(nullptr, "Echec", "Données non enregistrée");
        return;
    } else {
        QMessageBox::information(nullptr, "Succès", "Données correctement enregistrées");
    }
}


QSqlQueryModel* DatabaseConnection::rechercheEmploye(QMap<QString, QString> donnees) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query(mDatabase);

    if (donnees.isEmpty()) {
        QMessageBox::critical(nullptr, "Erreur", "Aucune condition fournie pour la recherche");
        return model;
    }

    QString debut = "SELECT * FROM employe WHERE ";
    QStringList conditions;

    for (const auto& cle : donnees.keys()) {
        conditions << cle + " = :" + cle;
    }

    QString requeteComplete = debut + conditions.join(" and ");
    query.prepare(requeteComplete);

    for (const auto& cle : donnees.keys()) {
        query.bindValue(":" + cle, donnees[cle]);
    }

    qDebug() << "Requête complète : " << requeteComplete;

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Échec", "Requête non exécutée : " + query.lastError().text());
        return model;
    }

    model->setQuery(std::move(query));
    return model;
}

void DatabaseConnection::modifieEmploye(QString id, QString nom, QString prenom, QString email, QString numero) {
    QSqlQuery query(mDatabase);
    QString requete = "UPDATE employe SET nom = :nom, prenom = :prenom, email = :email, numero = :numero WHERE id = :id";
    query.prepare(requete);
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":numero", numero);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Échec", "Modification non enregistrée: " + query.lastError().text());
        return;
    } else {
        QMessageBox::information(nullptr, "Succès", "Données correctement modifiées");
    }
}

void DatabaseConnection::supprimeEmploye(QString id){
    QString requete;
    QSqlQuery query;

    requete = "DELETE FROM employe where id=:id";

    query.prepare(requete);
    query.bindValue(":id", id);

    if (!(query.exec())){
        QMessageBox::critical(nullptr,"ECHEC", "Requête non executée");
        return;
    }

}
