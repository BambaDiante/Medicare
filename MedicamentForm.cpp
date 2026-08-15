#include "MedicamentForm.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


MedicamentForm::MedicamentForm(QWidget *parent)
    : QWidget(parent)
{


    gridLayout =
        new QGridLayout(this);




    titre =
        new QLabel("Ajouter un médicament");


    titre->setAlignment(Qt::AlignCenter);


    codeLabel =
        new QLabel("Code :");


    codeEdit =
        new QLineEdit();


    libelleLabel =
        new QLabel("Libellé :");


    libelleEdit =
        new QLineEdit();




    indicationLabel =
        new QLabel("Indication :");


    indicationEdit =
        new QTextEdit();




    posologieLabel =
        new QLabel("Posologie :");


    posologieEdit =
        new QTextEdit();


    ajouterButton =
        new QPushButton("Ajouter");



    gridLayout->addWidget(
        titre,
        0,
        0,
        1,
        2
        );


    gridLayout->addWidget(
        codeLabel,
        1,
        0
        );


    gridLayout->addWidget(
        codeEdit,
        1,
        1
        );


    gridLayout->addWidget(
        libelleLabel,
        2,
        0
        );


    gridLayout->addWidget(
        libelleEdit,
        2,
        1
        );


    gridLayout->addWidget(
        indicationLabel,
        3,
        0
        );


    gridLayout->addWidget(
        indicationEdit,
        3,
        1
        );


    gridLayout->addWidget(
        posologieLabel,
        4,
        0
        );


    gridLayout->addWidget(
        posologieEdit,
        4,
        1
        );


    gridLayout->addWidget(
        ajouterButton,
        5,
        0,
        1,
        2
        );


   //connexion en slots

    connect(
        ajouterButton,
        &QPushButton::clicked,
        this,
        &MedicamentForm::ajouterMedicament
        );
}


void MedicamentForm::ajouterMedicament()
{
    QString code =
        codeEdit->text().trimmed();


    QString libelle =
        libelleEdit->text().trimmed();


    QString indication =
        indicationEdit->toPlainText().trimmed();


    QString posologie =
        posologieEdit->toPlainText().trimmed();


    if (
        code.isEmpty() ||
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


  //Connexion a la base de de donnee
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
        "INSERT INTO Medicament "
        "(Code, libelle, Indications, Psologie) "
        "VALUES (:code, :libelle, :indications, :posologie)"
        );


    query.bindValue(
        ":code",
        code
        );


    query.bindValue(
        ":libelle",
        libelle
        );


    query.bindValue(
        ":indications",
        indication
        );


    query.bindValue(
        ":posologie",
        posologie
        );



    if (!query.exec())
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible d'ajouter le médicament.\n\n"
                + query.lastError().text()
            );

        return;
    }

//En cas de succes

    QMessageBox::information(
        this,
        "Succès",
        "Le médicament a été ajouté avec succès."
        );


    codeEdit->clear();

    libelleEdit->clear();

    indicationEdit->clear();

    posologieEdit->clear();

    codeEdit->setFocus();
}