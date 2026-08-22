#ifndef AJOUTPATIENTFORM_H
#define AJOUTPATIENTFORM_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QGridLayout;
class QPushButton;

class AjoutPatientForm : public QWidget
{
    Q_OBJECT

public:

    explicit AjoutPatientForm(QWidget *parent = nullptr);

signals:

    // Émis après l'ajout réussi d'un patient
    void patientAjoute();

private slots:

    void ajouterPatient();

private:

    QLabel *titre;

    QLabel *numSsLabel;
    QLineEdit *numSsEdit;

    QLabel *nomLabel;
    QLineEdit *nomEdit;

    QPushButton *ajouterButton;

    QGridLayout *gridLayout;
};

#endif // AJOUTPATIENTFORM_H