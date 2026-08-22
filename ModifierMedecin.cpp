#include "ModifierMedecin.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


ModifierMedecin::ModifierMedecin(
    int matriculeActuel,
    const QString &nomActuel,
    QWidget *parent
    )
    : QDialog(parent)
    , matriculeOriginal(matriculeActuel)
{

    setWindowTitle("Modifier le médecin");

    resize(350, 150);


    QVBoxLayout *layout =
        new QVBoxLayout(this);


    // =========================================
    // Titre
    // =========================================

    QLabel *titre =
        new QLabel("Modifier le médecin");


    titre->setAlignment(Qt::AlignCenter);


    layout->addWidget(titre);


    // =========================================
    // Formulaire
    // =========================================

    QFormLayout *formLayout =
        new QFormLayout();


    matriculeEdit =
        new QLineEdit(QString::number(matriculeActuel));

    nomEdit =
        new QLineEdit(nomActuel);


    formLayout->addRow("Matricule :", matriculeEdit);
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
        &ModifierMedecin::enregistrerModification
        );
}


void ModifierMedecin::enregistrerModification()
{
    QString matriculeTexte =
        matriculeEdit->text().trimmed();

    QString nom =
        nomEdit->text().trimmed();


    // =========================================
    // Validation
    // =========================================

    if (matriculeTexte.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Erreur",
            "Le matricule est obligatoire."
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


    bool matriculeValide = false;

    int nouveauMatricule =
        matriculeTexte.toInt(&matriculeValide);


    if (!matriculeValide)
    {
        QMessageBox::warning(
            this,
            "Erreur",
            "Le matricule doit être un nombre."
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
        "UPDATE Medecin "
        "SET Matricule = :nouveauMatricule, Nom = :nom "
        "WHERE Matricule = :matriculeOriginal"
        );


    query.bindValue(
        ":nouveauMatricule",
        nouveauMatricule
        );

    query.bindValue(
        ":nom",
        nom
        );

    query.bindValue(
        ":matriculeOriginal",
        matriculeOriginal
        );


    if (!query.exec())
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible de modifier le médecin.\n\n"
                + query.lastError().text()
            );

        return;
    }


    QMessageBox::information(
        this,
        "Succès",
        "Le médecin a été modifié avec succès."
        );


    accept();
}