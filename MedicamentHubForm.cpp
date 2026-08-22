#include "MedicamentHubForm.h"
#include "MedicamentForm.h"
#include "MedicamentListeForm.h"

#include <QVBoxLayout>
#include <QGroupBox>


MedicamentHubForm::MedicamentHubForm(QWidget *parent)
    : QWidget(parent)
{

    QVBoxLayout *layout =
        new QVBoxLayout(this);


    // =========================================
    // Cadran : ajouter un médicament
    // =========================================

    QGroupBox *cadreAjout =
        new QGroupBox("Ajouter un médicament");

    QVBoxLayout *layoutAjout =
        new QVBoxLayout(cadreAjout);


    medicamentForm =
        new MedicamentForm();

    layoutAjout->addWidget(medicamentForm);


    layout->addWidget(cadreAjout);


    // =========================================
    // Cadran : liste des médicaments
    // =========================================

    QGroupBox *cadreListe =
        new QGroupBox("Liste des médicaments");

    QVBoxLayout *layoutListe =
        new QVBoxLayout(cadreListe);


    medicamentListeForm =
        new MedicamentListeForm();

    layoutListe->addWidget(medicamentListeForm);


    layout->addWidget(cadreListe);


    // =========================================
    // Rafraîchissement automatique de la liste
    // =========================================

    connect(
        medicamentForm,
        &MedicamentForm::medicamentAjoute,
        medicamentListeForm,
        &MedicamentListeForm::chargerMedicaments
        );
}