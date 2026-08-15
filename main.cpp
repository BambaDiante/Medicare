#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // =========================================
    // Connexion à MySQL
    // =========================================

    QSqlDatabase db =
        QSqlDatabase::addDatabase("QMYSQL", "hospital_connection");

    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("hopital");
    db.setUserName("root");
    db.setPassword("");

    if (!db.open())
    {
        qDebug() << "Erreur de connexion MySQL :";
        qDebug() << db.lastError().text();

        return 1;
    }

    qDebug() << "Connexion MySQL réussie !";


    // =========================================
    // Fenêtre principale
    // =========================================

    MainWindow window;

    window.show();

    return app.exec();
}