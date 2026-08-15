#include "MedecinForm.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>


MedecinForm::MedecinForm(QWidget *parent)
    : QWidget(parent)
{


    gridLayout = new QGridLayout(this);




    titre =
        new QLabel("Ajouter un médecin");


    titre->setAlignment(Qt::AlignCenter);



    nameLabel =
        new QLabel("&Nom :");


    nameLineEdit =
        new QLineEdit();


    nameLabel->setBuddy(nameLineEdit);




    matriculeLabel =
        new QLabel("&Matricule :");


    matriculeLineEdit =
        new QLineEdit();


    matriculeLabel->setBuddy(matriculeLineEdit);


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
        matriculeLabel,
        2,
        0
        );


    gridLayout->addWidget(
        matriculeLineEdit,
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


    //Connexion du bouton en slot

    connect(
        valider,
        &QPushButton::clicked,
        this,
        &MedecinForm::validerFormulaire
        );
}



void MedecinForm::validerFormulaire()
{
    QString nom =
        nameLineEdit->text().trimmed();


    QString matricule =
        matriculeLineEdit->text().trimmed();



    if (nom.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Erreur",
            "Le nom est obligatoire."
            );

        return;
    }


    if (matricule.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Erreur",
            "Le matricule est obligatoire."
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
        "INSERT INTO Medecin "
        "(Nom, Matricule) "
        "VALUES (:nom, :matricule)"
        );


    query.bindValue(
        ":nom",
        nom
        );


    query.bindValue(
        ":matricule",
        matricule
        );


    // =========================================
    // Exécution
    // =========================================

    if (!query.exec())
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible d'enregistrer le médecin.\n\n"
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
        "Le médecin a été enregistré avec succès."
        );


    // =========================================
    // Réinitialisation
    // =========================================

    nameLineEdit->clear();

    matriculeLineEdit->clear();

    nameLineEdit->setFocus();
}