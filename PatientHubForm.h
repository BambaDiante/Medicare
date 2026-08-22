#ifndef PATIENTHUBFORM_H
#define PATIENTHUBFORM_H

#include <QWidget>

class AjoutPatientForm;
class PatientForm;

class PatientHubForm : public QWidget
{
    Q_OBJECT

public:

    explicit PatientHubForm(QWidget *parent = nullptr);

private:

    AjoutPatientForm *ajoutPatientForm;

    PatientForm *patientForm;
};

#endif // PATIENTHUBFORM_H