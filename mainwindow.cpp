#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databaseconnection.h"
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QRandomGenerator64>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db.testConnection();
    connect(ui->readButton, &QPushButton::clicked, this, &MainWindow::onReadButtonClicked);
    connect(ui->createButton, &QPushButton::clicked, this, &MainWindow::onCreateButtonClicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::onSearchButtonClicked);

    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::onUpdateButtonClicked);  // Nouvelle connexion
    connect(ui->tableView, &QTableView::clicked, this, &MainWindow::onTableViewClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteButtonclicked);
}

void MainWindow::onReadButtonClicked(){
    QSqlQueryModel* model = db.afficheEmploye();

    if(!model){
        QMessageBox::critical(this, "Le modèle n'existe pas","");
        return;
    }

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}


void MainWindow::onCreateButtonClicked(){
    //Générer un identifiant aléatoire
    int nombre = QRandomGenerator::global()->bounded(10000, 100000);
    char caractere = 'a' + QRandomGenerator::global()->bounded(26);

    QString id = QString::number(nombre) + caractere;
    QString nom = ui->lineEditNom->text();
    QString prenom = ui->lineEditPrenom->text();
    QString email = ui->lineEditEmail->text();
    QString numero = ui->lineEditNumero->text();

    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || numero.isEmpty()){
        QMessageBox::warning(this,"Tous les champs doivent être rensignés","");
        return;
    }

    db.ajouteEmploye(id, nom, prenom, email, numero);

}


void MainWindow::onSearchButtonClicked(){
    QSqlQueryModel* model = new QSqlQueryModel();

    QString nom = ui->lineEditNom->text();
    QString prenom =ui->lineEditPrenom->text();
    QString email =ui->lineEditEmail->text();
    QString numero =ui->lineEditNumero->text();

    if (nom.isEmpty() && prenom.isEmpty() && email.isEmpty() && numero.isEmpty()){
        delete model;
        return;
    }
    QMap<QString,QString> donnees;

    if (!(nom.isEmpty())){
        donnees.insert("nom", nom);
    }

    if (!(prenom.isEmpty())){
        donnees.insert("prenom", prenom);
    }

    if (!(email.isEmpty())){
        donnees.insert("email", email);
    }

    if (!(numero.isEmpty())){
        donnees.insert( "numero",numero);
    }

    model = db.rechercheEmploye(donnees);

    if (!model){
        QMessageBox::critical (this, "ECHEC", "Modèle inexistant");
        return;
    }

    ui->tableView->setModel(model);
}


void MainWindow::onUpdateButtonClicked() {
    QString nom = ui->lineEditNom->text();
    QString prenom = ui->lineEditPrenom->text();
    QString email = ui->lineEditEmail->text();
    QString numero = ui->lineEditNumero->text();

    //Je crér un modèle abstrait
    QAbstractItemModel *modelStatique = ui->tableView->model();
    // Je crée un modèle selectionnable
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();

    /*if (!selectionModel || !selectionModel->hasSelection()) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez d'abord sélectionner un employé à modifier");
        return;
    }*/

    QModelIndex idIndex = modelStatique->index(selectionModel->currentIndex().row(), 0); // Colonne 0 pour l'ID
    QString id = modelStatique->data(idIndex).toString();

    /*if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || numero.isEmpty()) {
        QMessageBox::warning(this, "Champs incomplets", "Tous les champs doivent être renseignés");
        return;
    }*/

    db.modifieEmploye(id, nom, prenom, email, numero);

    // Rafraîchir l'affichage après modification
    onReadButtonClicked();
}

void MainWindow::onTableViewClicked(const QModelIndex &index) {
    if (!index.isValid()) {
        qDebug() << "Index invalide";
        return;
    }

    QAbstractItemModel *model = ui->tableView->model();
    if (!model) {
        qDebug() << "Aucun modèle associé au QTableView";
        return;
    }

    /*if (model->columnCount() < 5) {
        qDebug() << "Erreur : le modèle n'a pas assez de colonnes";
        return;
    }*/

    int row = index.row();
    //QModelIndex idIndex = model->index(row, 0);
    QModelIndex nomIndex = model->index(row, 1);
    QModelIndex prenomIndex = model->index(row, 2);
    QModelIndex emailIndex = model->index(row, 3);
    QModelIndex numeroIndex = model->index(row, 4);

    ui->lineEditNom->setText(model->data(nomIndex).toString());
    ui->lineEditPrenom->setText(model->data(prenomIndex).toString());
    ui->lineEditEmail->setText(model->data(emailIndex).toString());
    ui->lineEditNumero->setText(model->data(numeroIndex).toString());
}

void MainWindow::onDeleteButtonclicked(){
    QAbstractItemModel* staticModel = ui->tableView->model();
    QItemSelectionModel* selectionModel = ui->tableView->selectionModel();

    if (!selectionModel){
        QMessageBox::critical (this, "ECHEC", "Aucune données selectionnées");
        return;
    }

    QModelIndex idIndex = staticModel->index(selectionModel->currentIndex().row(), 0);
    QString id = staticModel->data(idIndex).toString();

    db.supprimeEmploye(id);
    // Rafraîchir l'affichage après modification
    onReadButtonClicked();

}

MainWindow::~MainWindow()
{
    delete ui;
}
