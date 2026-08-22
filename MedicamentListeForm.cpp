#include "MedicamentListeForm.h"
#include "ModifierMedicament.h"

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


MedicamentListeForm::MedicamentListeForm(QWidget *parent)
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
        "Rechercher par code ou libellé..."
        );


    layout->addWidget(rechercheEdit);


    // =========================================
    // Tableau des médicaments
    // =========================================

    tableMedicaments =
        new QTableWidget();


    tableMedicaments->setColumnCount(4);

    tableMedicaments->setHorizontalHeaderLabels(
        {"Code", "Libellé", "Indications", "Posologie"}
        );

    tableMedicaments->horizontalHeader()->setStretchLastSection(true);

    tableMedicaments->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableMedicaments->setSelectionMode(QAbstractItemView::SingleSelection);

    tableMedicaments->setEditTriggers(QAbstractItemView::NoEditTriggers);


    layout->addWidget(tableMedicaments);


    // =========================================
    // Boutons d'action
    // =========================================

    QHBoxLayout *boutonsLayout =
        new QHBoxLayout();


    modifierButton =
        new QPushButton("Modifier");

    supprimerButton =
        new QPushButton("Supprimer");


    boutonsLayout->addWidget(modifierButton);
    boutonsLayout->addWidget(supprimerButton);


    layout->addLayout(boutonsLayout);


    // =========================================
    // Connexions
    // =========================================

    connect(
        modifierButton,
        &QPushButton::clicked,
        this,
        &MedicamentListeForm::modifierMedicament
        );

    connect(
        supprimerButton,
        &QPushButton::clicked,
        this,
        &MedicamentListeForm::supprimerMedicament
        );

    connect(
        rechercheEdit,
        &QLineEdit::textChanged,
        this,
        &MedicamentListeForm::filtrerMedicaments
        );


    chargerMedicaments();
}


void MedicamentListeForm::chargerMedicaments()
{
    tableMedicaments->setRowCount(0);


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
        "SELECT Code, libelle, Indications, Psologie FROM Medicament"
        );


    if (!query.exec())
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible de charger les médicaments.\n\n"
                + query.lastError().text()
            );

        return;
    }


    int row = 0;


    while (query.next())
    {
        tableMedicaments->insertRow(row);

        tableMedicaments->setItem(
            row,
            0,
            new QTableWidgetItem(query.value(0).toString())
            );

        tableMedicaments->setItem(
            row,
            1,
            new QTableWidgetItem(query.value(1).toString())
            );

        tableMedicaments->setItem(
            row,
            2,
            new QTableWidgetItem(query.value(2).toString())
            );

        tableMedicaments->setItem(
            row,
            3,
            new QTableWidgetItem(query.value(3).toString())
            );

        row++;
    }
}


void MedicamentListeForm::filtrerMedicaments(const QString &texte)
{
    for (int row = 0; row < tableMedicaments->rowCount(); row++)
    {
        QString code =
            tableMedicaments->item(row, 0)->text();

        QString libelle =
            tableMedicaments->item(row, 1)->text();

        bool correspond =
            code.contains(texte, Qt::CaseInsensitive)
            || libelle.contains(texte, Qt::CaseInsensitive);

        tableMedicaments->setRowHidden(row, !correspond);
    }
}


void MedicamentListeForm::modifierMedicament()
{
    int row =
        tableMedicaments->currentRow();


    if (row < 0)
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Veuillez sélectionner un médicament."
            );

        return;
    }


    QString code =
        tableMedicaments->item(row, 0)->text();

    QString libelle =
        tableMedicaments->item(row, 1)->text();

    QString indication =
        tableMedicaments->item(row, 2)->text();

    QString posologie =
        tableMedicaments->item(row, 3)->text();


    ModifierMedicament dialog(
        code,
        libelle,
        indication,
        posologie,
        this
        );


    if (dialog.exec() == QDialog::Accepted)
    {
        chargerMedicaments();
    }
}


void MedicamentListeForm::supprimerMedicament()
{
    int row =
        tableMedicaments->currentRow();


    if (row < 0)
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Veuillez sélectionner un médicament."
            );

        return;
    }


    QString code =
        tableMedicaments->item(row, 0)->text();

    QString libelle =
        tableMedicaments->item(row, 1)->text();


    auto reponse =
        QMessageBox::question(
            this,
            "Confirmer la suppression",
            "Voulez-vous vraiment supprimer le médicament \""
                + libelle + "\" ?\n\n"
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
        "DELETE FROM Medicament WHERE Code = :code"
        );


    query.bindValue(
        ":code",
        code
        );


    if (!query.exec())
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible de supprimer le médicament.\n\n"
                + query.lastError().text()
            );

        return;
    }


    QMessageBox::information(
        this,
        "Succès",
        "Le médicament a été supprimé."
        );


    chargerMedicaments();
}