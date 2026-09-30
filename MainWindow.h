#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QEvent>
#include <QVector>

class MedecinHubForm;
class MedicamentHubForm;
class PatientHubForm;
class ConsultationForm;
class QGraphicsOpacityEffect;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private:

    // =========================================
    // Navigation
    // =========================================

    QStackedWidget *stackedWidget;

    // =========================================
    // Pages
    // =========================================

    QWidget *accueilPage;

    MedecinHubForm *medecinHubForm;

    MedicamentHubForm *medicamentHubForm;

    PatientHubForm *patientHubForm;

    ConsultationForm *consultationForm;

    QGraphicsOpacityEffect *panneauOpacityEffect;

    // Effets d'opacité des cartes de l'accueil (un par carte),
    // utilisés pour l'animation d'apparition en cascade.
    QVector<QGraphicsOpacityEffect *> carteOpacityEffects;

    // =========================================
    // Boutons du menu
    // =========================================

    QPushButton *accueilButton;

    QPushButton *medecinButton;

    QPushButton *medicamentButton;

    QPushButton *patientButton;

    QPushButton *ajouterConsultationButton;

    // =========================================
    // Création du menu
    // =========================================

    QWidget *creerMenu();

    // =========================================
    // Création des pages
    // =========================================

    void creerPageAccueil();

private slots:

    // =========================================
    // Navigation
    // =========================================

    void afficherAccueil();
    void afficherMedecin();
    void afficherMedicament();
    void afficherPatient();
    void afficherFormulaireConsultation();
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
};



#endif // MAINWINDOW_H