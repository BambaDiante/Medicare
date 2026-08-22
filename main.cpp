#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include "MainWindow.h"
#include "LoginForm.h"
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile styleFile(":/style.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text))
    {
        QString style = QLatin1String(styleFile.readAll());
        app.setStyleSheet(style);
        styleFile.close();
    }

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
    // Authentification (cahier des charges, point 1)
    // =========================================
    LoginForm loginForm;
    if (loginForm.exec() != QDialog::Accepted)
    {
        // Échec de connexion ou fermeture du dialogue : on quitte
        // sans jamais afficher le menu principal.
        return 0;
    }

    // =========================================
    // Fenêtre principale
    // =========================================
    MainWindow window;
    window.show();
    return app.exec();
}