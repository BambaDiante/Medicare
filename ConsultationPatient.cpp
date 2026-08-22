#include "ConsultationPatient.h"
#include "DetailConsultation.h"

#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


ConsultationPatient::ConsultationPatient(
    int patientNumSs,
    const QString &nomPatient,
    QWidget *parent
    )
    : QDialog(parent)
{

    setWindowTitle("Consultations de " + nomPatient);

    resize(550, 450);


    QVBoxLayout *layout =
        new QVBoxLayout(this);


    // =========================================
    // Titre
    // =========================================

    QLabel *titre =
        new QLabel("Consultations de " + nomPatient);


    titre->setAlignment(Qt::AlignCenter);


    layout->addWidget(titre);


    // =========================================
    // Tableau des consultations
    // =========================================

    tableConsultations =
        new QTableWidget();


    tableConsultations->setColumnCount(4);

    tableConsultations->setHorizontalHeaderLabels(
        {"Numéro", "Date", "Médecin", "Motif"}
        );

    tableConsultations->horizontalHeader()->setStretchLastSection(true);

    tableConsultations->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableConsultations->setSelectionMode(QAbstractItemView::SingleSelection);

    tableConsultations->setEditTriggers(QAbstractItemView::NoEditTriggers);


    layout->addWidget(tableConsultations);


    // =========================================
    // Bouton détails
    // =========================================

    voirDetailsButton =
        new QPushButton("Voir détails");


    layout->addWidget(voirDetailsButton);


    connect(
        voirDetailsButton,
        &QPushButton::clicked,
        this,
        &ConsultationPatient::voirDetails
        );


    chargerConsultations(patientNumSs);
}


void ConsultationPatient::chargerConsultations(int patientNumSs)
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


    QSqlQuery query(db);


    query.prepare(
        "SELECT c.Numero, c.date, m.Nom, c.motif "
        "FROM Consultation c "
        "JOIN Medecin m "
        "  ON c.Medecin_has_Patient_Medecin_Matricule = m.Matricule "
        "WHERE c.Medecin_has_Patient_Patient_Num_ss = :numss "
        "ORDER BY c.date DESC"
        );


    query.bindValue(
        ":numss",
        patientNumSs
        );


    if (!query.exec())
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible de charger les consultations.\n\n"
                + query.lastError().text()
            );

        return;
    }


    int row = 0;


    while (query.next())
    {
        tableConsultations->insertRow(row);

        tableConsultations->setItem(
            row,
            0,
            new QTableWidgetItem(query.value(0).toString())
            );

        tableConsultations->setItem(
            row,
            1,
            new QTableWidgetItem(query.value(1).toString())
            );

        tableConsultations->setItem(
            row,
            2,
            new QTableWidgetItem(query.value(2).toString())
            );

        tableConsultations->setItem(
            row,
            3,
            new QTableWidgetItem(query.value(3).toString())
            );

        row++;
    }


    if (row == 0)
    {
        QMessageBox::information(
            this,
            "Information",
            "Ce patient n'a aucune consultation enregistrée."
            );
    }
}


void ConsultationPatient::voirDetails()
{
    int row =
        tableConsultations->currentRow();


    if (row < 0)
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Veuillez sélectionner une consultation."
            );

        return;
    }


    int numeroConsultation =
        tableConsultations->item(row, 0)->text().toInt();


    DetailConsultation dialog(numeroConsultation, this);

    dialog.exec();
}