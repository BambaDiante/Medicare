#ifndef MODIFIERMEDECIN_H
#define MODIFIERMEDECIN_H

#include <QDialog>

class QLineEdit;
class QPushButton;

class ModifierMedecin : public QDialog
{
    Q_OBJECT

public:

    // matriculeActuel : identifiant du médecin à modifier
    // nomActuel       : son nom actuel, pour pré-remplir le champ
    explicit ModifierMedecin(
        int matriculeActuel,
        const QString &nomActuel,
        QWidget *parent = nullptr
        );

private slots:

    void enregistrerModification();

private:

    int matriculeOriginal;

    QLineEdit *matriculeEdit;
    QLineEdit *nomEdit;

    QPushButton *enregistrerButton;
};

#endif // MODIFIERMEDECIN_H