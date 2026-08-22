#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>

class QLineEdit;
class QLabel;
class QPushButton;

class LoginForm : public QDialog
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);

private slots:
    void tenterConnexion();

private:
    void setupUi();

    QLineEdit   *loginEdit;
    QLineEdit   *motDePasseEdit;
    QLabel      *erreurLabel;
    QPushButton *connexionButton;
    QPushButton *quitterButton;
};

#endif // LOGINFORM_H