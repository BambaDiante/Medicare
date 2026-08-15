#ifndef PATIENTFORM_H
#define PATIENTFORM_H
#include <QWidget>

class QLabel;
class QLineEdit;
class QGridLayout;
class QPushButton;
class PatientForm : public QWidget
{
    Q_OBJECT
public:
    explicit PatientForm(QWidget *parent = nullptr);

private slots:

    void validerFormulaire();

private:

    QLabel *titre;

    QLabel *nameLabel;
    QLineEdit *nameLineEdit;

    QLabel *numeroLabel;
    QLineEdit *numeroLineEdit;

    QGridLayout *gridLayout;
    QPushButton *valider;


};

#endif // PATIENTFORM_H
