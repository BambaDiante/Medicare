#include "MedecinListeForm.h"
#include "ConsultationMedecin.h"
#include "ModifierMedecin.h"

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


MedecinListeForm::MedecinListeForm(QWidget *parent)
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
        "Rechercher par matricule ou nom..."
        );


    layout->addWidget(rechercheEdit);


    // =========================================
    // Tableau des médecins
    // =========================================

    tableMedecins =
        new QTableWidget();


    tableMedecins->setColumnCount(2);

    tableMedecins->setHorizontalHeaderLabels(
        {"Matricule", "Nom"}
        );

    tableMedecins->horizontalHeader()->setStretchLastSection(true);

    tableMedecins->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableMedecins->setSelectionMode(QAbstractItemView::SingleSelection);

    tableMedecins->setEditTriggers(QAbstractItemView::NoEditTriggers);


    layout->addWidget(tableMedecins);


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
        &MedecinListeForm::voirConsultations
        );

    connect(
        modifierButton,
        &QPushButton::clicked,
        this,
        &MedecinListeForm::modifierMedecin
        );

    connect(
        supprimerButton,
        &QPushButton::clicked,
        this,
        &MedecinListeForm::supprimerMedecin
        );

    connect(
        rechercheEdit,
        &QLineEdit::textChanged,
        this,
        &MedecinListeForm::filtrerMedecins
        );


    chargerMedecins();
}


void MedecinListeForm::chargerMedecins()
{
    tableMedecins->setRowCount(0);


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
        "SELECT Matricule, Nom FROM Medecin"
        );


    if (!query.exec())
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible de charger les médecins.\n\n"
                + query.lastError().text()
            );

        return;
    }


    int row = 0;


    while (query.next())
    {
        tableMedecins->insertRow(row);

        tableMedecins->setItem(
            row,
            0,
            new QTableWidgetItem(query.value(0).toString())
            );

        tableMedecins->setItem(
            row,
            1,
            new QTableWidgetItem(query.value(1).toString())
            );

        row++;
    }
}


void MedecinListeForm::filtrerMedecins(const QString &texte)
{
    for (int row = 0; row < tableMedecins->rowCount(); row++)
    {
        QString matricule =
            tableMedecins->item(row, 0)->text();

        QString nom =
            tableMedecins->item(row, 1)->text();

        bool correspond =
            matricule.contains(texte, Qt::CaseInsensitive)
            || nom.contains(texte, Qt::CaseInsensitive);

        tableMedecins->setRowHidden(row, !correspond);
    }
}


void MedecinListeForm::voirConsultations()
{
    int row =
        tableMedecins->currentRow();


    if (row < 0)
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Veuillez sélectionner un médecin."
            );

        return;
    }

    if (tableMedecins->isRowHidden(row))
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Veuillez sélectionner un médecin visible dans le tableau."
            );

        return;
    }


    int matricule =
        tableMedecins->item(row, 0)->text().toInt();

    QString nomMedecin =
        tableMedecins->item(row, 1)->text();


    ConsultationMedecin dialog(matricule, nomMedecin, this);

    dialog.exec();
}


void MedecinListeForm::modifierMedecin()
{
    int row =
        tableMedecins->currentRow();


    if (row < 0)
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Veuillez sélectionner un médecin."
            );

        return;
    }


    int matricule =
        tableMedecins->item(row, 0)->text().toInt();

    QString nomMedecin =
        tableMedecins->item(row, 1)->text();


    ModifierMedecin dialog(matricule, nomMedecin, this);


    if (dialog.exec() == QDialog::Accepted)
    {
        chargerMedecins();
    }
}


void MedecinListeForm::supprimerMedecin()
{
    int row =
        tableMedecins->currentRow();


    if (row < 0)
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Veuillez sélectionner un médecin."
            );

        return;
    }


    int matricule =
        tableMedecins->item(row, 0)->text().toInt();

    QString nomMedecin =
        tableMedecins->item(row, 1)->text();


    auto reponse =
        QMessageBox::question(
            this,
            "Confirmer la suppression",
            "Voulez-vous vraiment supprimer le médecin \""
                + nomMedecin + "\" ?\n\n"
                               "Cette action est irréversible.",
            QMessageBox::Yes | QMessageBox::No
            );

    if (reponse == QMessageBox::No)
    {
        return;
    }


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
        "DELETE FROM Medecin WHERE Matricule = :matricule"
        );


    query.bindValue(
        ":matricule",
        matricule
        );


    if (!query.exec())
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible de supprimer le médecin.\n\n"
                + query.lastError().text()
            );

        return;
    }


    QMessageBox::information(
        this,
        "Succès",
        "Le médecin a été supprimé."
        );


    chargerMedecins();
}