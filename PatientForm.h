#ifndef PATIENTFORM_H
#define PATIENTFORM_H

#include <QWidget>

class QTableWidget;
class QPushButton;
class QLineEdit;

class PatientForm : public QWidget
{
    Q_OBJECT

public:

    explicit PatientForm(QWidget *parent = nullptr);

public slots:

    void chargerPatients();

private slots:

    void voirConsultations();
    void filtrerPatients(const QString &texte);
    void modifierPatient();
    void supprimerPatient();

private:

    QLineEdit *rechercheEdit;

    QTableWidget *tablePatients;

    QPushButton *voirConsultationsButton;
    QPushButton *modifierButton;
    QPushButton *supprimerButton;
};

#endif // PATIENTFORM_H