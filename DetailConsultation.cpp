#include "DetailConsultation.h"

#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


DetailConsultation::DetailConsultation(
    int numeroConsultation,
    QWidget *parent
    )
    : QDialog(parent)
{

    setWindowTitle(
        QString("Détails de la consultation n°%1").arg(numeroConsultation)
        );

    resize(500, 350);


    QVBoxLayout *layout =
        new QVBoxLayout(this);


    // =========================================
    // Titre
    // =========================================

    QLabel *titre =
        new QLabel(
            QString("Médicaments prescrits - consultation n°%1")
                .arg(numeroConsultation)
            );


    titre->setAlignment(Qt::AlignCenter);

    titre->setWordWrap(true);


    layout->addWidget(titre);


    // =========================================
    // Tableau des médicaments prescrits
    // =========================================

    tableMedicaments =
        new QTableWidget();


    tableMedicaments->setColumnCount(3);

    tableMedicaments->setHorizontalHeaderLabels(
        {"Code", "Médicament", "Durée (jours)"}
        );

    tableMedicaments->horizontalHeader()->setStretchLastSection(true);

    tableMedicaments->setEditTriggers(QAbstractItemView::NoEditTriggers);


    layout->addWidget(tableMedicaments);


    chargerMedicaments(numeroConsultation);
}


void DetailConsultation::chargerMedicaments(int numeroConsultation)
{

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
    // Requête avec jointure
    // =========================================
    // On récupère les médicaments prescrits pour
    // cette consultation, avec leur libellé complet.

    QSqlQuery query(db);


    query.prepare(
        "SELECT p.Medicament_Code, m.libelle, p.nombre_de_jours "
        "FROM prescrit p "
        "JOIN Medicament m "
        "  ON p.Medicament_Code = m.Code "
        "WHERE p.Consultation_Numero = :numero"
        );


    query.bindValue(
        ":numero",
        numeroConsultation
        );


    if (!query.exec())
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible de charger les médicaments prescrits.\n\n"
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

        row++;
    }


    if (row == 0)
    {
        QMessageBox::information(
            this,
            "Information",
            "Aucun médicament n'a été prescrit pour cette consultation."
            );
    }
}