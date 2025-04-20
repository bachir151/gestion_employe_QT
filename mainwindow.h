#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QTableView>
#include "databaseconnection.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots :
    void onReadButtonClicked();
    void onCreateButtonClicked();
    void onSearchButtonClicked();
    void onUpdateButtonClicked();
    void onTableViewClicked(const QModelIndex &index);
    void onDeleteButtonclicked();

private:
    Ui::MainWindow *ui;
    DatabaseConnection db;
    QTableView* tableView;
};
#endif // MAINWINDOW_H
