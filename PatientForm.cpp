#include "PatientForm.h"
#include "ConsultationPatient.h"
#include "ModifierPatient.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


PatientForm::PatientForm(QWidget *parent)
    : QWidget(parent)
{

    QVBoxLayout *layout =
        new QVBoxLayout(this);


    // =========================================
    // Barre de recherche
    // =========================================

    rechercheEdit =
        new QLineEdit();

    rechercheEdit->setPlaceholderText(
        "Rechercher par numéro SS ou nom..."
        );


    layout->addWidget(rechercheEdit);


    // =========================================
    // Tableau des patients
    // =========================================

    tablePatients =
        new QTableWidget();


    tablePatients->setColumnCount(2);

    tablePatients->setHorizontalHeaderLabels(
        {"Numéro SS", "Nom"}
        );

    tablePatients->horizontalHeader()->setStretchLastSection(true);

    tablePatients->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablePatients->setSelectionMode(QAbstractItemView::SingleSelection);

    tablePatients->setEditTriggers(QAbstractItemView::NoEditTriggers);


    layout->addWidget(tablePatients);


    // =========================================
    // Boutons d'action
    // =========================================

    QHBoxLayout *boutonsLayout =
        new QHBoxLayout();


    voirConsultationsButton =
        new QPushButton("Voir les consultations");

    modifierButton =
        new QPushButton("Modifier");

    supprimerButton =
        new QPushButton("Supprimer");


    boutonsLayout->addWidget(voirConsultationsButton);
    boutonsLayout->addWidget(modifierButton);
    boutonsLayout->addWidget(supprimerButton);


    layout->addLayout(boutonsLayout);


    // =========================================
    // Connexions
    // =========================================

    connect(
        voirConsultationsButton,
        &QPushButton::clicked,
        this,
        &PatientForm::voirConsultations
        );

    connect(
        modifierButton,
        &QPushButton::clicked,
        this,
        &PatientForm::modifierPatient
        );

    connect(
        supprimerButton,
        &QPushButton::clicked,
        this,
        &PatientForm::supprimerPatient
        );

    connect(
        rechercheEdit,
        &QLineEdit::textChanged,
        this,
        &PatientForm::filtrerPatients
        );


    chargerPatients();
}


void PatientForm::chargerPatients()
{
    tablePatients->setRowCount(0);


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


    QSqlQuery query(db);


    query.prepare(
        "SELECT Num_ss, Nom FROM Patient"
        );


    if (!query.exec())
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible de charger les patients.\n\n"
                + query.lastError().text()
            );

        return;
    }


    int row = 0;


    while (query.next())
    {
        tablePatients->insertRow(row);

        tablePatients->setItem(
            row,
            0,
            new QTableWidgetItem(query.value(0).toString())
            );

        tablePatients->setItem(
            row,
            1,
            new QTableWidgetItem(query.value(1).toString())
            );

        row++;
    }
}


void PatientForm::filtrerPatients(const QString &texte)
{
    for (int row = 0; row < tablePatients->rowCount(); row++)
    {
        QString numSs =
            tablePatients->item(row, 0)->text();

        QString nom =
            tablePatients->item(row, 1)->text();

        bool correspond =
            numSs.contains(texte, Qt::CaseInsensitive)
            || nom.contains(texte, Qt::CaseInsensitive);

        tablePatients->setRowHidden(row, !correspond);
    }
}


void PatientForm::voirConsultations()
{
    int row =
        tablePatients->currentRow();


    if (row < 0)
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Veuillez sélectionner un patient."
            );

        return;
    }

    if (tablePatients->isRowHidden(row))
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Veuillez sélectionner un patient visible dans le tableau."
            );

        return;
    }


    int numSs =
        tablePatients->item(row, 0)->text().toInt();

    QString nomPatient =
        tablePatients->item(row, 1)->text();


    ConsultationPatient dialog(numSs, nomPatient, this);

    dialog.exec();
}


void PatientForm::modifierPatient()
{
    int row =
        tablePatients->currentRow();


    if (row < 0)
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Veuillez sélectionner un patient."
            );

        return;
    }


    int numSs =
        tablePatients->item(row, 0)->text().toInt();

    QString nomPatient =
        tablePatients->item(row, 1)->text();


    ModifierPatient dialog(numSs, nomPatient, this);


    // Si l'utilisateur a bien enregistré (accept()), on recharge le tableau
    if (dialog.exec() == QDialog::Accepted)
    {
        chargerPatients();
    }
}


void PatientForm::supprimerPatient()
{
    int row =
        tablePatients->currentRow();


    if (row < 0)
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Veuillez sélectionner un patient."
            );

        return;
    }


    int numSs =
        tablePatients->item(row, 0)->text().toInt();

    QString nomPatient =
        tablePatients->item(row, 1)->text();


    // =========================================
    // Confirmation avant suppression
    // =========================================

    auto reponse =
        QMessageBox::question(
            this,
            "Confirmer la suppression",
            "Voulez-vous vraiment supprimer le patient \""
                + nomPatient + "\" ?\n\n"
                               "Cette action est irréversible.",
            QMessageBox::Yes | QMessageBox::No
            );

    if (reponse == QMessageBox::No)
    {
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


    QSqlQuery query(db);


    query.prepare(
        "DELETE FROM Patient WHERE Num_ss = :numss"
        );


    query.bindValue(
        ":numss",
        numSs
        );


    if (!query.exec())
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible de supprimer le patient.\n\n"
                + query.lastError().text()
            );

        return;
    }


    QMessageBox::information(
        this,
        "Succès",
        "Le patient a été supprimé."
        );


    chargerPatients();
}