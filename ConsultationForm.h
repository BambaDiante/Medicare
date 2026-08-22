#ifndef CONSULTATIONFORM_H
#define CONSULTATIONFORM_H

#include <QWidget>

class QLabel;
class QDateEdit;
class QComboBox;
class QTextEdit;
class QSpinBox;
class QTableWidget;
class QGridLayout;
class QPushButton;
class QSqlRecord;

class ConsultationForm : public QWidget
{
    Q_OBJECT

public:
    explicit ConsultationForm(QWidget *parent = nullptr);

    // Recharge les 3 listes déroulantes (médecins, patients, médicaments)
    // À appeler à chaque ouverture de la page, pour voir les ajouts récents
    void actualiserListes();

private slots:
    void ajouterMedicamentALaListe();
    void retirerMedicamentDeLaListe();
    void enregistrerConsultation();

private:
    // Chargement des listes déroulantes depuis la base
    void chargerMedecins();
    void chargerPatients();
    void chargerMedicaments();

    // Transforme un QComboBox en barre de recherche
    void configurerRecherche(QComboBox *combo, const QString &texteIndicatif);

    // Contrôles
    bool medicamentDejaDansListe(const QString &code) const;

    // Accès base de données
    bool consultationAColonneMotif() const;
    bool assurerRelationMedecinPatient(int matricule, int numSS, QString &erreur);
    int  prochainNumeroConsultation(bool &ok);


    static QString colonneCorrespondante(const QSqlRecord &rec, const QStringList &candidats);

    void reinitialiserFormulaire();

    QLabel *titre;

    QLabel    *dateLabel;
    QDateEdit *dateEdit;

    QLabel    *medecinLabel;
    QComboBox *medecinCombo;

    QLabel    *patientLabel;
    QComboBox *patientCombo;

    QLabel    *motifLabel;
    QTextEdit *motifEdit;

    QLabel    *medicamentLabel;
    QComboBox *medicamentCombo;
    QLabel    *dureeLabel;
    QSpinBox  *dureeSpin;
    QPushButton *ajouterMedicamentButton;

    QTableWidget *medicamentsTable;
    QPushButton  *retirerMedicamentButton;

    QLabel *erreurLabel;

    QPushButton *valider;

    QGridLayout *gridLayout;
};

#endif // CONSULTATIONFORM_H