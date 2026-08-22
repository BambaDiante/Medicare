#include "ModifierPatient.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


ModifierPatient::ModifierPatient(
    int numSsActuel,
    const QString &nomActuel,
    QWidget *parent
    )
    : QDialog(parent)
    , numSsOriginal(numSsActuel)
{

    setWindowTitle("Modifier le patient");

    resize(350, 150);


    QVBoxLayout *layout =
        new QVBoxLayout(this);


    // =========================================
    // Titre
    // =========================================

    QLabel *titre =
        new QLabel("Modifier le patient");


    titre->setAlignment(Qt::AlignCenter);


    layout->addWidget(titre);


    // =========================================
    // Formulaire
    // =========================================

    QFormLayout *formLayout =
        new QFormLayout();


    numSsEdit =
        new QLineEdit(QString::number(numSsActuel));

    nomEdit =
        new QLineEdit(nomActuel);


    formLayout->addRow("Numéro SS :", numSsEdit);
    formLayout->addRow("Nom :", nomEdit);


    layout->addLayout(formLayout);


    // =========================================
    // Bouton
    // =========================================

    enregistrerButton =
        new QPushButton("Enregistrer");


    layout->addWidget(enregistrerButton);


    connect(
        enregistrerButton,
        &QPushButton::clicked,
        this,
        &ModifierPatient::enregistrerModification
        );
}


void ModifierPatient::enregistrerModification()
{
    QString numSsTexte =
        numSsEdit->text().trimmed();

    QString nom =
        nomEdit->text().trimmed();


    // =========================================
    // Validation
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

    int nouveauNumSs =
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
    // Requête de mise à jour
    // =========================================

    QSqlQuery query(db);


    query.prepare(
        "UPDATE Patient "
        "SET Num_ss = :nouveauNumSs, Nom = :nom "
        "WHERE Num_ss = :numSsOriginal"
        );


    query.bindValue(
        ":nouveauNumSs",
        nouveauNumSs
        );

    query.bindValue(
        ":nom",
        nom
        );

    query.bindValue(
        ":numSsOriginal",
        numSsOriginal
        );


    if (!query.exec())
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible de modifier le patient.\n\n"
                + query.lastError().text()
            );

        return;
    }


    QMessageBox::information(
        this,
        "Succès",
        "Le patient a été modifié avec succès."
        );


    // Ferme la fenêtre et signale le succès à PatientForm
    accept();
}