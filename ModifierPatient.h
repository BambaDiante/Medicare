#ifndef MODIFIERPATIENT_H
#define MODIFIERPATIENT_H

#include <QDialog>

class QLineEdit;
class QPushButton;

class ModifierPatient : public QDialog
{
    Q_OBJECT

public:

    // numSsActuel : identifiant du patient à modifier
    // nomActuel   : son nom actuel, pour pré-remplir le champ
    explicit ModifierPatient(
        int numSsActuel,
        const QString &nomActuel,
        QWidget *parent = nullptr
        );

private slots:

    void enregistrerModification();

private:

    int numSsOriginal;

    QLineEdit *numSsEdit;
    QLineEdit *nomEdit;

    QPushButton *enregistrerButton;
};

#endif // MODIFIERPATIENT_H