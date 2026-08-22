#include "MedecinHubForm.h"
#include "MedecinForm.h"
#include "MedecinListeForm.h"

#include <QVBoxLayout>
#include <QGroupBox>


MedecinHubForm::MedecinHubForm(QWidget *parent)
    : QWidget(parent)
{

    QVBoxLayout *layout =
        new QVBoxLayout(this);


    // =========================================
    // Cadran : ajouter un médecin
    // =========================================

    QGroupBox *cadreAjout =
        new QGroupBox("Ajouter un médecin");

    QVBoxLayout *layoutAjout =
        new QVBoxLayout(cadreAjout);


    medecinForm =
        new MedecinForm();

    layoutAjout->addWidget(medecinForm);


    layout->addWidget(cadreAjout);


    // =========================================
    // Cadran : liste des médecins
    // =========================================

    QGroupBox *cadreListe =
        new QGroupBox("Liste des médecins");

    QVBoxLayout *layoutListe =
        new QVBoxLayout(cadreListe);


    medecinListeForm =
        new MedecinListeForm();

    layoutListe->addWidget(medecinListeForm);


    layout->addWidget(cadreListe);


    // =========================================
    // Rafraîchissement automatique de la liste
    // =========================================

    connect(
        medecinForm,
        &MedecinForm::medecinAjoute,
        medecinListeForm,
        &MedecinListeForm::chargerMedecins
        );
}