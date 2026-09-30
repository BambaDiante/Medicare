#include "MainWindow.h"
#include "MedecinHubForm.h"
#include "MedicamentHubForm.h"
#include "PatientHubForm.h"
#include "ConsultationForm.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QFont>
#include <QEvent>
#include <QResizeEvent>
#include <QPixmap>
#include <QIcon>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QVariantAnimation>
#include <QColor>
#include <QFrame>


// ---------------------------------------------------------------------------
// QLabel spécialisé : garde toujours une image nette, redimensionnée en
// mode "cover" (comme background-size: cover en CSS), à chaque changement
// de taille du widget.
// ---------------------------------------------------------------------------
class ImageFondLabel : public QLabel
{
public:
    explicit ImageFondLabel(QWidget *parent = nullptr)
        : QLabel(parent)
    {
        setMinimumSize(1, 1);
    }

    void definirImage(const QPixmap &image)
    {
        imageOriginale = image;
        mettreAJourAffichage();
    }

protected:
    void resizeEvent(QResizeEvent *event) override
    {
        QLabel::resizeEvent(event);
        mettreAJourAffichage();
    }

private:
    QPixmap imageOriginale;

    void mettreAJourAffichage()
    {
        if (imageOriginale.isNull() || width() <= 0 || height() <= 0)
        {
            return;
        }

        QPixmap redimensionnee = imageOriginale.scaled(
            size(),
            Qt::KeepAspectRatioByExpanding,
            Qt::SmoothTransformation
            );

        int x = (redimensionnee.width() - width()) / 2;
        int y = (redimensionnee.height() - height()) / 2;

        QPixmap recadree = redimensionnee.copy(x, y, width(), height());

        setPixmap(recadree);
    }
};


// ---------------------------------------------------------------------------
// Dessine une icône (forme colorée) sur un badge circulaire,
// pour les cartes de l'accueil.
// ---------------------------------------------------------------------------
static QPixmap creerIconeBadge(const QString &type, const QColor &couleurFond)
{
    const int taille = 64;

    QPixmap pixmap(taille, taille);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(couleurFond);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, taille, taille);

    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::white, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    if (type == "medecin")
    {
        painter.setBrush(Qt::NoBrush);
        painter.drawArc(18, 16, 28, 28, 0, 180 * 16);
        painter.drawLine(46, 30, 46, 40);
        painter.setBrush(Qt::white);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(41, 38, 10, 10);
        painter.drawEllipse(15, 12, 8, 8);
        painter.drawEllipse(29, 12, 8, 8);
    }
    else if (type == "medicament")
    {
        painter.save();
        painter.translate(taille / 2.0, taille / 2.0);
        painter.rotate(-45);

        QRectF capsule(-20, -9, 40, 18);

        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        painter.drawRoundedRect(capsule, 9, 9);

        painter.setBrush(couleurFond.darker(115));
        QRectF moitie(-20, -9, 20, 18);
        painter.setClipRect(moitie);
        painter.drawRoundedRect(capsule, 9, 9);

        painter.restore();
    }
    else if (type == "patient")
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        painter.drawEllipse(22, 12, 20, 20);

        QPainterPath corps;
        corps.addRoundedRect(14, 34, 36, 20, 10, 10);
        painter.drawPath(corps);
    }
    else if (type == "consultation")
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        painter.drawRoundedRect(16, 14, 32, 38, 4, 4);

        painter.setBrush(couleurFond);
        painter.drawRoundedRect(24, 10, 16, 8, 3, 3);

        painter.setPen(QPen(couleurFond, 3, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(21, 26, 39, 26);
        painter.drawLine(21, 33, 39, 33);
        painter.drawLine(21, 40, 33, 40);
    }
    else if (type == "accueil")
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);

        QPainterPath toit;
        toit.moveTo(32, 14);
        toit.lineTo(48, 28);
        toit.lineTo(44, 28);
        toit.lineTo(44, 32);
        toit.lineTo(20, 32);
        toit.lineTo(20, 28);
        toit.lineTo(16, 28);
        toit.closeSubpath();
        painter.drawPath(toit);

        painter.drawRoundedRect(20, 30, 24, 18, 2, 2);

        painter.setBrush(couleurFond);
        painter.drawRect(28, 38, 8, 10);
    }

    painter.end();

    return pixmap;
}


// ---------------------------------------------------------------------------
// Dessine une icône monochrome blanche (sans badge circulaire), plus petite,
// destinée à être affichée devant le texte des boutons du menu latéral.
// ---------------------------------------------------------------------------
static QIcon creerIconeMenu(const QString &type)
{
    const int taille = 28;

    QPixmap pixmap(taille, taille);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    if (type == "accueil")
    {
        painter.setPen(Qt::NoPen);

        QPainterPath toit;
        toit.moveTo(14, 4);
        toit.lineTo(24, 12);
        toit.lineTo(21, 12);
        toit.lineTo(21, 14);
        toit.lineTo(7, 14);
        toit.lineTo(7, 12);
        toit.lineTo(4, 12);
        toit.closeSubpath();
        painter.drawPath(toit);

        painter.drawRoundedRect(7, 13, 14, 10, 1, 1);
    }
    else if (type == "medecin")
    {
        painter.setBrush(Qt::NoBrush);
        painter.drawArc(6, 6, 14, 14, 0, 180 * 16);
        painter.drawLine(20, 13, 20, 18);
        painter.setBrush(Qt::white);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(17, 16, 6, 6);
        painter.drawEllipse(4, 3, 5, 5);
        painter.drawEllipse(13, 3, 5, 5);
    }
    else if (type == "medicament")
    {
        painter.save();
        painter.translate(taille / 2.0, taille / 2.0);
        painter.rotate(-45);

        QRectF capsule(-10, -4.5, 20, 9);

        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        painter.drawRoundedRect(capsule, 4.5, 4.5);

        painter.restore();
    }
    else if (type == "patient")
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        painter.drawEllipse(9, 4, 10, 10);

        QPainterPath corps;
        corps.addRoundedRect(5, 16, 18, 10, 5, 5);
        painter.drawPath(corps);
    }
    else if (type == "consultation")
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        painter.drawRoundedRect(6, 5, 16, 19, 2, 2);

        painter.setBrush(QColor("#0E7C74"));
        painter.drawRoundedRect(10, 3, 8, 4, 1, 1);

        painter.setPen(QPen(QColor("#0E7C74"), 1.6, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(9, 12, 19, 12);
        painter.drawLine(9, 16, 19, 16);
        painter.drawLine(9, 20, 15, 20);
    }

    painter.end();

    return QIcon(pixmap);
}


// Taille normale et taille agrandie des cartes de l'accueil,
// utilisées pour l'effet de zoom au survol.
static const QSize TAILLE_CARTE_NORMALE(190, 170);
static const QSize TAILLE_CARTE_SURVOL(202, 182);


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //Fenetre Principale
    setWindowTitle("MediCare");

    resize(1000, 600);

    // Le style visuel général est géré par style.qss
    // (chargé dans main.cpp via app.setStyleSheet).


    stackedWidget = new QStackedWidget(this);


    //creation des pages

    creerPageAccueil();


    //creation des formulaire

    medecinHubForm = new MedecinHubForm();

    medicamentHubForm = new MedicamentHubForm();

    patientHubForm = new PatientHubForm();

    consultationForm = new ConsultationForm();

    consultationForm->setObjectName("consultationForm");


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
    QWidget *menuExterne = new QWidget();

    menuExterne->setObjectName("menuExterne");

    menuExterne->setFixedWidth(300);


    QStackedLayout *superposition =
        new QStackedLayout(menuExterne);

    superposition->setStackingMode(QStackedLayout::StackAll);

    superposition->setContentsMargins(0, 0, 0, 0);


    ImageFondLabel *imageFondMenu = new ImageFondLabel();

    imageFondMenu->definirImage(QPixmap(":/medicale.jpg"));


    QWidget *menu = new QWidget();

    menu->setObjectName("menu");


    QVBoxLayout *layout = new QVBoxLayout(menu);

    layout->setContentsMargins(20, 25, 20, 20);

    layout->setSpacing(6);


    QLabel *titreMenu =
        new QLabel("GESTION HOSPITALIERE");

    titreMenu->setObjectName("titreMenu");

    titreMenu->setWordWrap(true);


    QFont font;

    font.setBold(true);

    font.setPointSize(15);


    titreMenu->setFont(font);

    titreMenu->setAlignment(Qt::AlignCenter);


    QFrame *separateur = new QFrame();

    separateur->setObjectName("separateurMenu");

    separateur->setFixedHeight(2);


    layout->addWidget(titreMenu);

    layout->addSpacing(10);

    layout->addWidget(separateur);

    layout->addSpacing(20);


    accueilButton =
        new QPushButton("  Accueil");

    accueilButton->setIcon(creerIconeMenu("accueil"));


    medecinButton =
        new QPushButton("  Médecin");

    medecinButton->setIcon(creerIconeMenu("medecin"));


    medicamentButton =
        new QPushButton("  Médicament");

    medicamentButton->setIcon(creerIconeMenu("medicament"));


    patientButton =
        new QPushButton("  Patient");

    patientButton->setIcon(creerIconeMenu("patient"));


    ajouterConsultationButton =
        new QPushButton("  Ajouter une consultation");

    ajouterConsultationButton->setIcon(creerIconeMenu("consultation"));


    for (QPushButton *bouton : {
             accueilButton,
             medecinButton,
             medicamentButton,
             patientButton,
             ajouterConsultationButton
         })
    {
        bouton->setIconSize(QSize(20, 20));
        bouton->setCursor(Qt::PointingHandCursor);
        layout->addWidget(bouton);
    }


    layout->addStretch();


    superposition->addWidget(imageFondMenu);
    superposition->addWidget(menu);

    superposition->setCurrentWidget(menu);


    return menuExterne;
}


//creation de la page d'acceuil

void MainWindow::creerPageAccueil()
{
    accueilPage = new QWidget();

    accueilPage->setObjectName("accueilPage");


    QStackedLayout *superposition =
        new QStackedLayout(accueilPage);

    superposition->setStackingMode(QStackedLayout::StackAll);

    superposition->setContentsMargins(0, 0, 0, 0);


    ImageFondLabel *imageFond = new ImageFondLabel();

    imageFond->definirImage(QPixmap(":/medicale.jpg"));

    imageFond->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding
        );


    QWidget *panneau = new QWidget();

    panneau->setObjectName("panneauAccueil");


    panneauOpacityEffect = new QGraphicsOpacityEffect(panneau);

    panneauOpacityEffect->setOpacity(1.0);

    panneau->setGraphicsEffect(panneauOpacityEffect);


    QVBoxLayout *layout =
        new QVBoxLayout(panneau);

    layout->setContentsMargins(60, 50, 60, 50);

    layout->setSpacing(6);


    QLabel *titre = new QLabel("Bienvenue");
    titre->setObjectName("titreAccueil");
    QFont fontTitre;
    fontTitre.setPointSize(34);
    fontTitre.setBold(true);
    titre->setFont(fontTitre);
    titre->setAlignment(Qt::AlignCenter);

    QLabel *sousTitre = new QLabel("Au coeur de votre établissement,une gestion plus intelligente");
    sousTitre->setObjectName("sousTitreAccueil");
    sousTitre->setAlignment(Qt::AlignCenter);

    QFrame *barreAccent = new QFrame();
    barreAccent->setObjectName("barreAccentAccueil");
    barreAccent->setFixedSize(70, 4);


    QHBoxLayout *barreLayout = new QHBoxLayout();
    barreLayout->addStretch();
    barreLayout->addWidget(barreAccent);
    barreLayout->addStretch();


    // =========================================
    // Cartes de raccourcis (avec effet de zoom au survol)
    // =========================================
    QHBoxLayout *cartesLayout = new QHBoxLayout();
    cartesLayout->setSpacing(20);

    auto creerCarte = [this](
                          const QString &type,
                          const QColor &couleur,
                          const QString &titreCarte,
                          const QString &cible
                          )
    {
        QWidget *carte = new QWidget();
        carte->setObjectName("carteAccueil");
        carte->setProperty("cibleNavigation", cible);
        carte->setFixedSize(TAILLE_CARTE_NORMALE);
        carte->setCursor(Qt::PointingHandCursor);
        carte->installEventFilter(this);

        QVBoxLayout *carteLayout = new QVBoxLayout(carte);
        carteLayout->setAlignment(Qt::AlignCenter);
        carteLayout->setSpacing(14);

        QLabel *iconeLabel = new QLabel();
        iconeLabel->setObjectName("iconeCarteAccueil");
        iconeLabel->setAlignment(Qt::AlignCenter);
        iconeLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
        iconeLabel->setPixmap(creerIconeBadge(type, couleur));

        QLabel *titreLabel = new QLabel(titreCarte);
        titreLabel->setObjectName("titreCarteAccueil");
        titreLabel->setAlignment(Qt::AlignCenter);
        titreLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

        carteLayout->addWidget(iconeLabel);
        carteLayout->addWidget(titreLabel);

        return carte;
    };

    const QColor couleurPrincipale("#0E9B8E");

    cartesLayout->addStretch();
    cartesLayout->addWidget(creerCarte("medecin", couleurPrincipale, "Médecins", "medecin"));
    cartesLayout->addWidget(creerCarte("medicament", couleurPrincipale, "Médicaments", "medicament"));
    cartesLayout->addWidget(creerCarte("patient", couleurPrincipale, "Patients", "patient"));
    cartesLayout->addWidget(creerCarte("consultation", couleurPrincipale, "Consultations", "consultation"));
    cartesLayout->addStretch();



    layout->addStretch();
    layout->addWidget(titre);
    layout->addWidget(sousTitre);
    layout->addSpacing(14);
    layout->addLayout(barreLayout);
    layout->addSpacing(40);
    layout->addLayout(cartesLayout);
    layout->addStretch();

    superposition->addWidget(imageFond);
    superposition->addWidget(panneau);

    superposition->setCurrentWidget(panneau);

    stackedWidget->addWidget(accueilPage);
}


void MainWindow::afficherAccueil()
{
    stackedWidget->setCurrentWidget(accueilPage);

    QPropertyAnimation *animation =
        new QPropertyAnimation(panneauOpacityEffect, "opacity", this);

    animation->setDuration(500);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setEasingCurve(QEasingCurve::OutCubic);

    animation->start(QAbstractAnimation::DeleteWhenStopped);
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
    QWidget *carte = qobject_cast<QWidget*>(watched);

    if (carte && carte->property("cibleNavigation").isValid())
    {
        // =========================================
        // Clic sur la carte : navigation
        // =========================================
        if (event->type() == QEvent::MouseButtonRelease)
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
            else if (cible == "medicament")
            {
                afficherMedicament();
            }
            else if (cible == "consultation")
            {
                afficherFormulaireConsultation();
            }

            return true;
        }

        // =========================================
        // Survol de la carte : effet de zoom doux
        // =========================================
        if (event->type() == QEvent::Enter)
        {
            QVariantAnimation *anim = new QVariantAnimation(carte);
            anim->setDuration(150);
            anim->setStartValue(carte->size());
            anim->setEndValue(TAILLE_CARTE_SURVOL);
            anim->setEasingCurve(QEasingCurve::OutCubic);

            connect(
                anim,
                &QVariantAnimation::valueChanged,
                carte,
                [carte](const QVariant &valeur)
                {
                    carte->setFixedSize(valeur.toSize());
                }
                );

            anim->start(QAbstractAnimation::DeleteWhenStopped);
        }
        else if (event->type() == QEvent::Leave)
        {
            QVariantAnimation *anim = new QVariantAnimation(carte);
            anim->setDuration(150);
            anim->setStartValue(carte->size());
            anim->setEndValue(TAILLE_CARTE_NORMALE);
            anim->setEasingCurve(QEasingCurve::OutCubic);

            connect(
                anim,
                &QVariantAnimation::valueChanged,
                carte,
                [carte](const QVariant &valeur)
                {
                    carte->setFixedSize(valeur.toSize());
                }
                );

            anim->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }

    return QMainWindow::eventFilter(watched, event);
}