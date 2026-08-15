#include "PatientForm.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>

PatientForm::PatientForm(QWidget *parent)
    : QWidget(parent)
{


    gridLayout = new QGridLayout(this);




    titre =
        new QLabel("Ajouter un Patient");


    titre->setAlignment(Qt::AlignCenter);



    nameLabel =
        new QLabel("&Nom :");


    nameLineEdit =
        new QLineEdit();


    nameLabel->setBuddy(nameLineEdit);




    numeroLabel =
        new QLabel("&Numero :");


    numeroLineEdit =
        new QLineEdit();


    numeroLabel->setBuddy(numeroLineEdit);


    //bouton

    valider =
        new QPushButton("Valider");



    gridLayout->addWidget(
        titre,
        0,
        0,
        1,
        2
        );


    gridLayout->addWidget(
        nameLabel,
        1,
        0
        );


    gridLayout->addWidget(
        nameLineEdit,
        1,
        1
        );


    gridLayout->addWidget(
        numeroLabel,
        2,
        0
        );


    gridLayout->addWidget(
        numeroLineEdit,
        2,
        1
        );


    gridLayout->addWidget(
        valider,
        3,
        0,
        1,
        2
        );



    connect(
        valider,
        &QPushButton::clicked,
        this,
        &PatientForm::validerFormulaire
        );
}
void PatientForm::validerFormulaire()
{
    QString nom =
        nameLineEdit->text().trimmed();


    QString numero =
        numeroLineEdit->text().trimmed();



    if (nom.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Erreur",
            "Le nom est obligatoire."
            );

        return;
    }


    if (numero.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Erreur",
            "Le numero est obligatoire."
            );

        return;
    }

    //COnnexion

    QSqlDatabase db =
        QSqlDatabase::database(
            "hospital_connection"
            );


    if (!db.isOpen())
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "La connexion à la base de données "
            "est fermée."
            );

        return;
    }


    // =========================================
    // Requête
    // =========================================

    QSqlQuery query(db);


    query.prepare(
        "INSERT INTO patient "
        "(Nom, Numero) "
        "VALUES (:nom, :num)"
        );


    query.bindValue(
        ":nom",
        nom
        );


    query.bindValue(
        ":num",
        numero
        );


    // =========================================
    // Exécution
    // =========================================

    if (!query.exec())
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible d'enregistrer le patient.\n\n"
                + query.lastError().text()
            );

        qDebug()
            << query.lastError().text();

        return;
    }


    // =========================================
    // Succès
    // =========================================

    QMessageBox::information(
        this,
        "Succès",
        "Le patient a été enregistré avec succès."
        );


    // =========================================
    // Réinitialisation
    // =========================================

    nameLineEdit->clear();

    numeroLineEdit->clear();

    nameLineEdit->setFocus();
}



