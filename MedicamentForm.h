#ifndef MEDICAMENTFORM_H
#define MEDICAMENTFORM_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QTextEdit;
class QGridLayout;
class QPushButton;

class MedicamentForm : public QWidget
{
    Q_OBJECT

public:

    explicit MedicamentForm(QWidget *parent = nullptr);

signals:

    // Émis après l'ajout réussi d'un médicament
    void medicamentAjoute();

private slots:

    void ajouterMedicament();

private:

    QLabel *titre;

    QLabel *codeLabel;
    QLineEdit *codeEdit;

    QLabel *libelleLabel;
    QLineEdit *libelleEdit;

    QLabel *indicationLabel;
    QTextEdit *indicationEdit;

    QLabel *posologieLabel;
    QTextEdit *posologieEdit;

    QPushButton *ajouterButton;

    QGridLayout *gridLayout;
};

#endif // MEDICAMENTFORM_H