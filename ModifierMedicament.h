#ifndef MODIFIERMEDICAMENT_H
#define MODIFIERMEDICAMENT_H

#include <QDialog>

class QLineEdit;
class QTextEdit;
class QPushButton;

class ModifierMedicament : public QDialog
{
    Q_OBJECT

public:

    // codeActuel : identifiant du médicament à modifier
    explicit ModifierMedicament(
        const QString &codeActuel,
        const QString &libelleActuel,
        const QString &indicationActuelle,
        const QString &posologieActuelle,
        QWidget *parent = nullptr
        );

private slots:

    void enregistrerModification();

private:

    QString codeOriginal;

    QLineEdit *codeEdit;
    QLineEdit *libelleEdit;
    QTextEdit *indicationEdit;
    QTextEdit *posologieEdit;

    QPushButton *enregistrerButton;
};

#endif // MODIFIERMEDICAMENT_H