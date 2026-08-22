#ifndef DETAILCONSULTATION_H
#define DETAILCONSULTATION_H

#include <QDialog>

class QTableWidget;

class DetailConsultation : public QDialog
{
    Q_OBJECT

public:

    // numeroConsultation : identifie la consultation dont on veut voir les médicaments prescrits
    explicit DetailConsultation(
        int numeroConsultation,
        QWidget *parent = nullptr
        );

private:

    QTableWidget *tableMedicaments;

    void chargerMedicaments(int numeroConsultation);
};

#endif // DETAILCONSULTATION_H