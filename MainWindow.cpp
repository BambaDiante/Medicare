#include "MainWindow.h"
#include "MedecinHubForm.h"
#include "MedicamentHubForm.h"
#include "PatientHubForm.h"
#include "ConsultationForm.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>
#include <QEvent>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //Fenetre Principale
    setWindowTitle("Gestion d'un centre médical");

    resize(1000, 600);


    stackedWidget = new QStackedWidget(this);


    //creation des pages

    creerPageAccueil();


    //creation des formulaire

    medecinHubForm = new MedecinHubForm();

    medicamentHubForm = new MedicamentHubForm();

    patientHubForm = new PatientHubForm();

    consultationForm = new ConsultationForm();


    //Ajout des formulaires au stackwidget

    stackedWidget->addWidget(medecinHubForm);

    stackedWidget->addWidget(medicamentHubForm);

    stackedWidget->addWidget(patientHubForm);

    stackedWidget->addWidget(consultationForm);


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
        medecinButton,
        &QPushButton::clicked,
        this,
        &MainWindow::afficherMedecin
        );

    connect(
        medicamentButton,
        &QPushButton::clicked,
        this,
        &MainWindow::afficherMedicament
        );

    connect(
        patientButton,
        &QPushButton::clicked,
        this,
        &MainWindow::afficherPatient
        );

    connect(
        ajouterConsultationButton,
        &QPushButton::clicked,
        this,
        &MainWindow::afficherFormulaireConsultation
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


    medecinButton =
        new QPushButton("Médecin");


    medicamentButton =
        new QPushButton("Médicament");


    patientButton =
        new QPushButton("Patient");


    ajouterConsultationButton =
        new QPushButton("Ajouter une consultation");


    //ajout des bouttons

    layout->addWidget(accueilButton);

    layout->addWidget(medecinButton);

    layout->addWidget(medicamentButton);

    layout->addWidget(patientButton);

    layout->addWidget(ajouterConsultationButton);


    layout->addStretch();


    return menu;
}


//creation de la page d'acceuil

void MainWindow::creerPageAccueil()
{
    accueilPage = new QWidget();

    QVBoxLayout *layout = new QVBoxLayout(accueilPage);
    layout->setContentsMargins(60, 60, 60, 60);
    layout->setSpacing(20);

    // =========================================
    // Image d'illustration
    // =========================================
    QLabel *image = new QLabel();
    QPixmap pixmap(":/accueil.png");
    image->setPixmap(
        pixmap.scaledToWidth(320, Qt::SmoothTransformation)
        );
    image->setAlignment(Qt::AlignCenter);

    // =========================================
    // Titre
    // =========================================
    QLabel *titre = new QLabel("Bienvenue");
    QFont fontTitre;
    fontTitre.setPointSize(28);
    fontTitre.setBold(true);
    titre->setFont(fontTitre);
    titre->setAlignment(Qt::AlignCenter);

    // =========================================
    // Description
    // =========================================
    QLabel *description = new QLabel(
        "Gérez vos patients, médecins, médicaments et "
        "consultations depuis une seule interface."
        );
    description->setAlignment(Qt::AlignCenter);
    description->setStyleSheet("color: #5A6A6A; font-size: 14px;");
    description->setWordWrap(true);

    // =========================================
    // Cartes de raccourcis
    // =========================================
    QHBoxLayout *cartesLayout = new QHBoxLayout();
    cartesLayout->setSpacing(16);

    auto creerCarte = [this](const QString &titreCarte, const QString &desc, const QString &cible)
    {
        QWidget *carte = new QWidget();
        carte->setObjectName("carteAccueil");
        carte->setProperty("cibleNavigation", cible);
        carte->setFixedSize(200, 120);
        carte->setCursor(Qt::PointingHandCursor);
        carte->installEventFilter(this);

        QVBoxLayout *carteLayout = new QVBoxLayout(carte);

        QLabel *titreLabel = new QLabel(titreCarte);
        titreLabel->setStyleSheet("font-weight: bold; font-size: 15px;");
        titreLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

        QLabel *descLabel = new QLabel(desc);
        descLabel->setStyleSheet("color: #5A6A6A;");
        descLabel->setWordWrap(true);
        descLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

        carteLayout->addWidget(titreLabel);
        carteLayout->addWidget(descLabel);
        carteLayout->addStretch();

        return carte;
    };

    cartesLayout->addStretch();
    cartesLayout->addWidget(creerCarte("Patients", "Gérer les dossiers", "patient"));
    cartesLayout->addWidget(creerCarte("Médecins", "Gérer les praticiens", "medecin"));
    cartesLayout->addWidget(creerCarte("Consultations", "Ajouter un rendez-vous", "consultation"));
    cartesLayout->addStretch();



    // =========================================
    // Assemblage
    // =========================================
    layout->addStretch();
    layout->addWidget(image);
    layout->addWidget(titre);
    layout->addWidget(description);
    layout->addSpacing(30);
    layout->addLayout(cartesLayout);
    layout->addStretch();

    stackedWidget->addWidget(accueilPage);
}


void MainWindow::afficherAccueil()
{
    stackedWidget->setCurrentWidget(accueilPage);
}


void MainWindow::afficherMedecin()
{
    stackedWidget->setCurrentWidget(medecinHubForm);
}


void MainWindow::afficherMedicament()
{
    stackedWidget->setCurrentWidget(medicamentHubForm);
}


void MainWindow::afficherPatient()
{
    stackedWidget->setCurrentWidget(patientHubForm);
}


void MainWindow::afficherFormulaireConsultation()
{
    consultationForm->actualiserListes();

    stackedWidget->setCurrentWidget(consultationForm);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease)
    {
        QWidget *carte = qobject_cast<QWidget*>(watched);

        if (carte && carte->property("cibleNavigation").isValid())
        {
            QString cible = carte->property("cibleNavigation").toString();

            if (cible == "patient")
            {
                afficherPatient();
            }
            else if (cible == "medecin")
            {
                afficherMedecin();
            }
            else if (cible == "consultation")
            {
                afficherFormulaireConsultation();
            }

            return true;
        }
    }

    return QMainWindow::eventFilter(watched, event);
}