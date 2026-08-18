#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
class MedecinForm;
class MedicamentForm;
class PatientForm;
class ConsultationForm;
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
    MedecinForm *medecinForm;
    MedicamentForm *medicamentForm;
    PatientForm *patientForm;
    ConsultationForm *consultationForm;
    QWidget *patientsPage;
    // =========================================
    // Boutons du menu
    // =========================================
    QPushButton *accueilButton;
    QPushButton *ajouterMedecinButton;
    QPushButton *ajouterMedicamentButton;
    QPushButton *ajouterPatientButton;
    QPushButton *ajouterConsultationButton;
    QPushButton *patientsButton;
    // =========================================
    // Création du menu
    // =========================================
    QWidget *creerMenu();
    // =========================================
    // Création des pages
    // =========================================
    void creerPageAccueil();
    void creerPagePatients();
private slots:
    // =========================================
    // Navigation
    // =========================================
    void afficherAccueil();
    void afficherFormulaireMedecin();
    void afficherFormulaireMedicament();
    void afficherFormulairePatient();
    void afficherFormulaireConsultation();
    void afficherPatients();
};
#endif // MAINWINDOW_H