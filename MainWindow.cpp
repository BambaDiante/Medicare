#include "MainWindow.h"

#include "MedecinForm.h"
#include "MedicamentForm.h"
#include "PatientForm.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //Fenetre Principale
    setWindowTitle("Gestion d'un centre médical");

    resize(1000, 600);




    stackedWidget = new QStackedWidget(this);


    //creation des pages

    creerPageAccueil();

    creerPagePatients();


    //creation des formulaire

    medecinForm = new MedecinForm();

    medicamentForm = new MedicamentForm();

    patientForm = new PatientForm();


    //Ajout des formulaires au stackwidget

    stackedWidget->addWidget(medecinForm);

    stackedWidget->addWidget(medicamentForm);

    stackedWidget->addWidget(patientForm);




    //creation de menu

    QWidget *menu = creerMenu();


    //widget central

    QWidget *centralWidget = new QWidget(this);

    QHBoxLayout *layoutPrincipal =
        new QHBoxLayout(centralWidget);

    layoutPrincipal->setContentsMargins(0, 0, 0, 0);

    layoutPrincipal->setSpacing(0);



    layoutPrincipal->addWidget(menu);



    layoutPrincipal->addWidget(stackedWidget);



    setCentralWidget(centralWidget);


    //connexion des boutons

    connect(
        accueilButton,
        &QPushButton::clicked,
        this,
        &MainWindow::afficherAccueil
        );


    connect(
        ajouterMedecinButton,
        &QPushButton::clicked,
        this,
        &MainWindow::afficherFormulaireMedecin
        );


    connect(
        ajouterMedicamentButton,
        &QPushButton::clicked,
        this,
        &MainWindow::afficherFormulaireMedicament
        );

    connect(
        ajouterPatientButton,
        &QPushButton::clicked,
        this,
        &MainWindow::afficherFormulairePatient
        );


    connect(
        patientsButton,
        &QPushButton::clicked,
        this,
        &MainWindow::afficherPatients
        );


    //page affiche au demarrage

    afficherAccueil();
}


//definition du menu
QWidget *MainWindow::creerMenu()
{
    QWidget *menu = new QWidget();

    menu->setObjectName("menu");

    menu->setFixedWidth(300);


    QVBoxLayout *layout = new QVBoxLayout(menu);

    layout->setContentsMargins(15, 20, 15, 20);

    layout->setSpacing(10);



    QLabel *titreMenu =
        new QLabel("GESTION HOSPITALIERE");


    QFont font;

    font.setBold(true);

    font.setPointSize(16);


    titreMenu->setFont(font);

    titreMenu->setAlignment(Qt::AlignCenter);


    layout->addWidget(titreMenu);

    layout->addSpacing(30);


    accueilButton =
        new QPushButton("Accueil");



    ajouterMedecinButton =
        new QPushButton("Ajouter un médecin");


    ajouterMedicamentButton =
        new QPushButton("Ajouter un médicament");

    ajouterPatientButton=
        new QPushButton("Ajouter un patient");



    patientsButton =
        new QPushButton("Patients");


    //ajout des bouttons

    layout->addWidget(accueilButton);

    layout->addWidget(ajouterMedecinButton);

    layout->addWidget(ajouterMedicamentButton);

    layout->addWidget(ajouterPatientButton);

    layout->addWidget(patientsButton);



    layout->addStretch();


    return menu;
}


//creation de la page d'acceuil

void MainWindow::creerPageAccueil()
{
    accueilPage = new QWidget();


    QVBoxLayout *layout =
        new QVBoxLayout(accueilPage);


    QLabel *titre =
        new QLabel("Bienvenue");


    QFont font;

    font.setPointSize(28);

    font.setBold(true);


    titre->setFont(font);

    titre->setAlignment(Qt::AlignCenter);



    QLabel *description =
        new QLabel(
            "Bienvenue dans votre application "
            "de gestion de centre médical."
            );


    description->setAlignment(Qt::AlignCenter);



    layout->addStretch();

    layout->addWidget(titre);

    layout->addWidget(description);

    layout->addStretch();



    stackedWidget->addWidget(accueilPage);
}





void MainWindow::creerPagePatients()
{
    patientsPage = new QWidget();


    QVBoxLayout *layout =
        new QVBoxLayout(patientsPage);


    QLabel *titre =
        new QLabel("Gestion des patients");


    QFont font;

    font.setPointSize(24);

    font.setBold(true);


    titre->setFont(font);

    titre->setAlignment(Qt::AlignCenter);


    layout->addStretch();

    layout->addWidget(titre);

    layout->addStretch();


    stackedWidget->addWidget(patientsPage);
}


void MainWindow::afficherAccueil()
{
    stackedWidget->setCurrentWidget(accueilPage);
}




void MainWindow::afficherFormulaireMedecin()
{
    stackedWidget->setCurrentWidget(medecinForm);
}




void MainWindow::afficherFormulaireMedicament()
{
    stackedWidget->setCurrentWidget(medicamentForm);
}

void MainWindow::afficherFormulairePatient()
{
    stackedWidget->setCurrentWidget(patientForm);
}



void MainWindow::afficherPatients()
{
    stackedWidget->setCurrentWidget(patientsPage);
}