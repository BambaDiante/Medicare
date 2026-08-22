#include "PatientHubForm.h"
#include "AjoutPatientForm.h"
#include "PatientForm.h"

#include <QVBoxLayout>
#include <QGroupBox>


PatientHubForm::PatientHubForm(QWidget *parent)
    : QWidget(parent)
{

    QVBoxLayout *layout =
        new QVBoxLayout(this);


    // =========================================
    // Cadran : ajouter un patient
    // =========================================

    QGroupBox *cadreAjout =
        new QGroupBox("Ajouter un patient");

    QVBoxLayout *layoutAjout =
        new QVBoxLayout(cadreAjout);


    ajoutPatientForm =
        new AjoutPatientForm();

    layoutAjout->addWidget(ajoutPatientForm);


    layout->addWidget(cadreAjout);


    // =========================================
    // Cadran : liste des patients
    // =========================================

    QGroupBox *cadreListe =
        new QGroupBox("Liste des patients");

    QVBoxLayout *layoutListe =
        new QVBoxLayout(cadreListe);


    patientForm =
        new PatientForm();

    layoutListe->addWidget(patientForm);


    layout->addWidget(cadreListe);


    // =========================================
    // Rafraîchissement automatique de la liste
    // =========================================

    connect(
        ajoutPatientForm,
        &AjoutPatientForm::patientAjoute,
        patientForm,
        &PatientForm::chargerPatients
        );
}