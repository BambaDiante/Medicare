#include "AjoutPatientForm.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>


AjoutPatientForm::AjoutPatientForm(QWidget *parent)
    : QWidget(parent)
{

    gridLayout = new QGridLayout(this);


    // =========================================
    // Titre
    // =========================================

    titre =
        new QLabel("Ajouter un patient");


    titre->setAlignment(Qt::AlignCenter);


    // =========================================
    // Champs du formulaire
    // =========================================

    numSsLabel =
        new QLabel("&Numéro SS :");

    numSsEdit =
        new QLineEdit();

    numSsLabel->setBuddy(numSsEdit);


    nomLabel =
        new QLabel("&Nom :");

    nomEdit =
        new QLineEdit();

    nomLabel->setBuddy(nomEdit);


    // =========================================
    // Bouton
    // =========================================

    ajouterButton =
        new QPushButton("Ajouter");


    // =========================================
    // Placement dans la grille
    // =========================================

    gridLayout->addWidget(
        titre,
        0,
        0,
        1,
        2
        );

    gridLayout->addWidget(
        numSsLabel,
        1,
        0
        );

    gridLayout->addWidget(
        numSsEdit,
        1,
        1
        );

    gridLayout->addWidget(
        nomLabel,
        2,
        0
        );

    gridLayout->addWidget(
        nomEdit,
        2,
        1
        );

    gridLayout->addWidget(
        ajouterButton,
        3,
        0,
        1,
        2
        );


    // =========================================
    // Connexion du bouton en slot
    // =========================================

    connect(
        ajouterButton,
        &QPushButton::clicked,
        this,
        &AjoutPatientForm::ajouterPatient
        );
}


void AjoutPatientForm::ajouterPatient()
{
    QString numSsTexte =
        numSsEdit->text().trimmed();

    QString nom =
        nomEdit->text().trimmed();


    // =========================================
    // Validation des champs
    // =========================================

    if (numSsTexte.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Erreur",
            "Le numéro de sécurité sociale est obligatoire."
            );

        return;
    }

    if (nom.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Erreur",
            "Le nom est obligatoire."
            );

        return;
    }


    bool numSsValide = false;

    int numSs =
        numSsTexte.toInt(&numSsValide);


    if (!numSsValide)
    {
        QMessageBox::warning(
            this,
            "Erreur",
            "Le numéro de sécurité sociale doit être un nombre."
            );

        return;
    }


    // =========================================
    // Connexion à la base
    // =========================================

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
        "INSERT INTO Patient "
        "(Num_ss, Nom) "
        "VALUES (:numss, :nom)"
        );


    query.bindValue(
        ":numss",
        numSs
        );


    query.bindValue(
        ":nom",
        nom
        );


    // =========================================
    // Exécution
    // =========================================

    if (!query.exec())
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible d'ajouter le patient.\n\n"
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
        "Le patient a été ajouté avec succès."
        );

    emit patientAjoute();


    // =========================================
    // Réinitialisation
    // =========================================

    numSsEdit->clear();

    nomEdit->clear();

    numSsEdit->setFocus();
}