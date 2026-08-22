#include "LoginForm.h"

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFont>
#include <QCryptographicHash>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// Même connexion nommée que le reste de l'application (voir main.cpp)
static const char *CONNEXION_BD = "hospital_connection";

LoginForm::LoginForm(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Connexion - Gestion hospitalière");
    setFixedWidth(360);
    setupUi();
}

void LoginForm::setupUi()
{
    auto *layoutPrincipal = new QVBoxLayout(this);

    auto *titre = new QLabel("Connexion");
    QFont font = titre->font();
    font.setBold(true);
    font.setPointSize(18);
    titre->setFont(font);
    titre->setAlignment(Qt::AlignCenter);
    layoutPrincipal->addWidget(titre);
    layoutPrincipal->addSpacing(20);

    auto *formLayout = new QFormLayout();

    loginEdit = new QLineEdit(this);
    loginEdit->setPlaceholderText("Identifiant");
    formLayout->addRow("Login :", loginEdit);

    motDePasseEdit = new QLineEdit(this);
    motDePasseEdit->setPlaceholderText("Mot de passe");
    motDePasseEdit->setEchoMode(QLineEdit::Password);
    formLayout->addRow("Mot de passe :", motDePasseEdit);

    layoutPrincipal->addLayout(formLayout);

    erreurLabel = new QLabel(this);
    erreurLabel->setStyleSheet("color: red;");
    erreurLabel->setWordWrap(true);
    layoutPrincipal->addWidget(erreurLabel);

    auto *ligneBoutons = new QHBoxLayout();
    quitterButton = new QPushButton("Quitter", this);
    connexionButton = new QPushButton("Se connecter", this);
    connexionButton->setDefault(true);
    ligneBoutons->addWidget(quitterButton);
    ligneBoutons->addStretch();
    ligneBoutons->addWidget(connexionButton);
    layoutPrincipal->addLayout(ligneBoutons);

    loginEdit->setFocus();

    // =========================================
    // Connexion des signaux
    // =========================================
    connect(
        connexionButton,
        &QPushButton::clicked,
        this,
        &LoginForm::tenterConnexion
        );

    connect(
        quitterButton,
        &QPushButton::clicked,
        this,
        &QDialog::reject
        );

    // Permet de valider avec la touche Entrée depuis n'importe quel champ
    connect(loginEdit, &QLineEdit::returnPressed, this, &LoginForm::tenterConnexion);
    connect(motDePasseEdit, &QLineEdit::returnPressed, this, &LoginForm::tenterConnexion);
}

void LoginForm::tenterConnexion()
{
    erreurLabel->clear();

    QString login = loginEdit->text().trimmed();
    QString motDePasse = motDePasseEdit->text();

    if (login.isEmpty() || motDePasse.isEmpty())
    {
        erreurLabel->setText("Veuillez renseigner le login et le mot de passe.");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database(CONNEXION_BD);
    if (!db.isOpen())
    {
        erreurLabel->setText("La connexion à la base de données est fermée.");
        return;
    }

    // On ne stocke/compare jamais le mot de passe en clair.
    QString hash = QString(
        QCryptographicHash::hash(motDePasse.toUtf8(), QCryptographicHash::Sha256).toHex()
        );

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM utilisateur WHERE login = :login AND mot_de_passe = :motDePasse");
    query.bindValue(":login", login);
    query.bindValue(":motDePasse", hash);

    if (!query.exec() || !query.next())
    {
        erreurLabel->setText("Erreur lors de la vérification des identifiants.");
        qDebug() << query.lastError().text();
        return;
    }

    if (query.value(0).toInt() == 0)
    {
        erreurLabel->setText("Login ou mot de passe incorrect.");
        motDePasseEdit->clear();
        motDePasseEdit->setFocus();
        return;
    }

    // Identifiants valides : on ferme le dialogue avec succès.
    accept();
}