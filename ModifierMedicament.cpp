#include "ModifierMedicament.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


ModifierMedicament::ModifierMedicament(
    const QString &codeActuel,
    const QString &libelleActuel,
    const QString &indicationActuelle,
    const QString &posologieActuelle,
    QWidget *parent
    )
    : QDialog(parent)
    , codeOriginal(codeActuel)
{

    setWindowTitle("Modifier le médicament");

    resize(400, 350);


    QVBoxLayout *layout =
        new QVBoxLayout(this);


    // =========================================
    // Titre
    // =========================================

    QLabel *titre =
        new QLabel("Modifier le médicament");


    titre->setAlignment(Qt::AlignCenter);


    layout->addWidget(titre);


    // =========================================
    // Formulaire
    // =========================================

    QFormLayout *formLayout =
        new QFormLayout();


    codeEdit =
        new QLineEdit(codeActuel);

    libelleEdit =
        new QLineEdit(libelleActuel);

    indicationEdit =
        new QTextEdit(indicationActuelle);

    indicationEdit->setMaximumHeight(80);

    posologieEdit =
        new QTextEdit(posologieActuelle);

    posologieEdit->setMaximumHeight(80);


    formLayout->addRow("Code :", codeEdit);
    formLayout->addRow("Libellé :", libelleEdit);
    formLayout->addRow("Indications :", indicationEdit);
    formLayout->addRow("Posologie :", posologieEdit);


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
        &ModifierMedicament::enregistrerModification
        );
}


void ModifierMedicament::enregistrerModification()
{
    QString nouveauCode =
        codeEdit->text().trimmed();

    QString libelle =
        libelleEdit->text().trimmed();

    QString indication =
        indicationEdit->toPlainText().trimmed();

    QString posologie =
        posologieEdit->toPlainText().trimmed();


    // =========================================
    // Validation
    // =========================================

    if (
        nouveauCode.isEmpty() ||
        libelle.isEmpty() ||
        indication.isEmpty() ||
        posologie.isEmpty()
        )
    {
        QMessageBox::warning(
            this,
            "Erreur",
            "Veuillez remplir tous les champs."
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
        "UPDATE Medicament "
        "SET Code = :nouveauCode, "
        "libelle = :libelle, "
        "Indications = :indication, "
        "Psologie = :posologie "
        "WHERE Code = :codeOriginal"
        );


    query.bindValue(
        ":nouveauCode",
        nouveauCode
        );

    query.bindValue(
        ":libelle",
        libelle
        );

    query.bindValue(
        ":indication",
        indication
        );

    query.bindValue(
        ":posologie",
        posologie
        );

    query.bindValue(
        ":codeOriginal",
        codeOriginal
        );


    if (!query.exec())
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible de modifier le médicament.\n\n"
                + query.lastError().text()
            );

        return;
    }


    QMessageBox::information(
        this,
        "Succès",
        "Le médicament a été modifié avec succès."
        );


    accept();
}