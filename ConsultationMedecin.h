#ifndef CONSULTATIONMEDECIN_H
#define CONSULTATIONMEDECIN_H

#include <QDialog>

class QTableWidget;
class QDateEdit;
class QPushButton;
class QCheckBox;

class ConsultationMedecin : public QDialog
{
    Q_OBJECT

public:

    explicit ConsultationMedecin(
        int medecinMatricule,
        const QString &nomMedecin,
        QWidget *parent = nullptr
        );

private slots:

    // Recharge le tableau selon la plage de dates choisie
    void filtrerParDate();

    // Affiche toutes les consultations, sans filtre de date
    void reinitialiserFiltre();

private:

    int matricule;

    QCheckBox *filtreActifCheckBox;

    QDateEdit *dateDebutEdit;
    QDateEdit *dateFinEdit;

    QPushButton *filtrerButton;
    QPushButton *reinitialiserButton;

    QTableWidget *tableConsultations;

    // avecFiltre = false -> ignore les dates, ramène tout l'historique
    void chargerConsultations(bool avecFiltre);
};

#endif // CONSULTATIONMEDECIN_H