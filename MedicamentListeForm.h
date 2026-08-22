#ifndef MEDICAMENTLISTEFORM_H
#define MEDICAMENTLISTEFORM_H

#include <QWidget>

class QTableWidget;
class QPushButton;
class QLineEdit;

class MedicamentListeForm : public QWidget
{
    Q_OBJECT

public:

    explicit MedicamentListeForm(QWidget *parent = nullptr);

public slots:

    void chargerMedicaments();

private slots:

    void filtrerMedicaments(const QString &texte);
    void modifierMedicament();
    void supprimerMedicament();

private:

    QLineEdit *rechercheEdit;

    QTableWidget *tableMedicaments;

    QPushButton *modifierButton;
    QPushButton *supprimerButton;
};

#endif // MEDICAMENTLISTEFORM_H