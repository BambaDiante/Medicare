#ifndef CONSULTATIONPATIENT_H
#define CONSULTATIONPATIENT_H

#include <QDialog>

class QTableWidget;
class QPushButton;

class ConsultationPatient : public QDialog
{
    Q_OBJECT

public:

    explicit ConsultationPatient(
        int patientNumSs,
        const QString &nomPatient,
        QWidget *parent = nullptr
        );

private slots:

    // Ouvre le détail (médicaments prescrits) de la consultation sélectionnée
    void voirDetails();

private:

    QTableWidget *tableConsultations;

    QPushButton *voirDetailsButton;

    void chargerConsultations(int patientNumSs);
};

#endif // CONSULTATIONPATIENT_H