#ifndef MEDECINLISTEFORM_H
#define MEDECINLISTEFORM_H

#include <QWidget>

class QTableWidget;
class QPushButton;
class QLineEdit;

class MedecinListeForm : public QWidget
{
    Q_OBJECT

public:

    explicit MedecinListeForm(QWidget *parent = nullptr);

public slots:

    void chargerMedecins();

private slots:

    void voirConsultations();
    void modifierMedecin();
    void supprimerMedecin();
    void filtrerMedecins(const QString &texte);

private:

    QLineEdit *rechercheEdit;

    QTableWidget *tableMedecins;

    QPushButton *voirConsultationsButton;
    QPushButton *modifierButton;
    QPushButton *supprimerButton;
};

#endif // MEDECINLISTEFORM_H