#ifndef MEDECINFORM_H
#define MEDECINFORM_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QGridLayout;
class QPushButton;

class MedecinForm : public QWidget
{
    Q_OBJECT

public:
    explicit MedecinForm(QWidget *parent = nullptr);

signals:

    // Émis après l'ajout réussi d'un médecin
    void medecinAjoute();

private slots:

    void validerFormulaire();

private:

    QLabel *titre;

    QLabel *nameLabel;
    QLineEdit *nameLineEdit;

    QLabel *matriculeLabel;
    QLineEdit *matriculeLineEdit;

    QGridLayout *gridLayout;

    QPushButton *valider;
};


#endif // MEDECINFORM_H