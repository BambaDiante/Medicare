#include "ConsultationMedecin.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLabel>
#include <QDateEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QMessageBox>
#include <QDate>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>


ConsultationMedecin::ConsultationMedecin(
    int medecinMatricule,
    const QString &nomMedecin,
    QWidget *parent
    )
    : QDialog(parent)
    , matricule(medecinMatricule)
{

    setWindowTitle("Consultations du Dr " + nomMedecin);

    resize(600, 450);


    QVBoxLayout *layout =
        new QVBoxLayout(this);


    // =========================================
    // Titre
    // =========================================

    QLabel *titre =
        new QLabel("Consultations du Dr " + nomMedecin);


    titre->setAlignment(Qt::AlignCenter);


    layout->addWidget(titre);


    // =========================================
    // Zone de filtre par date
    // =========================================

    QHBoxLayout *filtreLayout =
        new QHBoxLayout();


    filtreActifCheckBox =
        new QCheckBox("Filtrer par date");

    // Par défaut : aucun filtre, on affiche tout l'historique
    filtreActifCheckBox->setChecked(false);


    QLabel *deLabel =
        new QLabel("Du :");

    dateDebutEdit =
        new QDateEdit(QDate::currentDate());

    dateDebutEdit->setCalendarPopup(true);
    dateDebutEdit->setDisplayFormat("dd/MM/yyyy");


    QLabel *aLabel =
        new QLabel("Au :");

    dateFinEdit =
        new QDateEdit(QDate::currentDate());

    dateFinEdit->setCalendarPopup(true);
    dateFinEdit->setDisplayFormat("dd/MM/yyyy");


    filtrerButton =
        new QPushButton("Filtrer");

    reinitialiserButton =
        new QPushButton("Tout afficher");


    filtreLayout->addWidget(filtreActifCheckBox);
    filtreLayout->addWidget(deLabel);
    filtreLayout->addWidget(dateDebutEdit);
    filtreLayout->addWidget(aLabel);
    filtreLayout->addWidget(dateFinEdit);
    filtreLayout->addWidget(filtrerButton);
    filtreLayout->addWidget(reinitialiserButton);


    layout->addLayout(filtreLayout);


    // =========================================
    // Tableau des consultations
    // =========================================

    tableConsultations =
        new QTableWidget();

    tableConsultations->horizontalHeader()->setStretchLastSection(true);

    tableConsultations->setEditTriggers(QAbstractItemView::NoEditTriggers);


    layout->addWidget(tableConsultations);


    // =========================================
    // Connexions
    // =========================================

    connect(
        filtrerButton,
        &QPushButton::clicked,
        this,
        &ConsultationMedecin::filtrerParDate
        );

    connect(
        reinitialiserButton,
        &QPushButton::clicked,
        this,
        &ConsultationMedecin::reinitialiserFiltre
        );


    // Chargement initial : tout l'historique, sans filtre
    chargerConsultations(false);
}


void ConsultationMedecin::filtrerParDate()
{
    if (dateDebutEdit->date() > dateFinEdit->date())
    {
        QMessageBox::warning(
            this,
            "Erreur",
            "La date de début doit être antérieure "
            "ou égale à la date de fin."
            );

        return;
    }

    chargerConsultations(true);
}


void ConsultationMedecin::reinitialiserFiltre()
{
    filtreActifCheckBox->setChecked(false);

    chargerConsultations(false);
}


void ConsultationMedecin::chargerConsultations(bool avecFiltre)
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
    // Détection de la colonne "motif"
    // =========================================
    // Certaines bases n'ont pas encore cette colonne,
    // on l'affiche seulement si elle existe.

    QSqlRecord recConsultation =
        db.record("Consultation");

    bool avecMotif =
        recConsultation.indexOf("motif") >= 0;


    // =========================================
    // Construction du tableau (colonnes)
    // =========================================

    tableConsultations->setRowCount(0);

    if (avecMotif)
    {
        tableConsultations->setColumnCount(4);

        tableConsultations->setHorizontalHeaderLabels(
            {"Numéro", "Date", "Patient", "Motif"}
            );
    }
    else
    {
        tableConsultations->setColumnCount(3);

        tableConsultations->setHorizontalHeaderLabels(
            {"Numéro", "Date", "Patient"}
            );
    }


    // =========================================
    // Requête
    // =========================================

    QSqlQuery query(db);


    QString sql =
        "SELECT c.Numero, c.date, p.Nom ";

    if (avecMotif)
    {
        sql += ", c.motif ";
    }

    sql +=
        "FROM Consultation c "
        "JOIN Patient p "
        "  ON c.Medecin_has_Patient_Patient_Num_ss = p.Num_ss "
        "WHERE c.Medecin_has_Patient_Medecin_Matricule = :matricule ";

    if (avecFiltre)
    {
        sql += "AND c.date BETWEEN :dateDebut AND :dateFin ";
    }

    sql += "ORDER BY c.date DESC";


    query.prepare(sql);


    query.bindValue(
        ":matricule",
        matricule
        );


    if (avecFiltre)
    {
        query.bindValue(
            ":dateDebut",
            dateDebutEdit->date().toString("yyyy-MM-dd")
            );

        query.bindValue(
            ":dateFin",
            dateFinEdit->date().toString("yyyy-MM-dd")
            );
    }


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


    // =========================================
    // Remplissage du tableau
    // =========================================

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

        if (avecMotif)
        {
            tableConsultations->setItem(
                row,
                3,
                new QTableWidgetItem(query.value(3).toString())
                );
        }

        row++;
    }


    if (row == 0)
    {
        QMessageBox::information(
            this,
            "Information",
            "Aucune consultation trouvée pour cette période."
            );
    }
}