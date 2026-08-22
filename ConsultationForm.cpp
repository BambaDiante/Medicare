#include "ConsultationForm.h"

#include <QGridLayout>
#include <QLabel>
#include <QDateEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QSpinBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QDate>
#include <QCompleter>
#include <QLineEdit>
#include <QAbstractItemView>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

// Nom de la connexion ouverte dans main.cpp :
//   QSqlDatabase::addDatabase("QMYSQL", "hospital_connection");
static const char *CONNEXION_BD = "hospital_connection";

ConsultationForm::ConsultationForm(QWidget *parent)
    : QWidget(parent)
{
    gridLayout = new QGridLayout(this);

    titre = new QLabel("Ajouter une consultation");
    titre->setAlignment(Qt::AlignCenter);

    // --- Date ---
    dateLabel = new QLabel("&Date :");
    dateEdit = new QDateEdit(QDate::currentDate());
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    dateEdit->setMaximumDate(QDate::currentDate());
    dateLabel->setBuddy(dateEdit);

    // --- Médecin ---
    medecinLabel = new QLabel("&Médecin :");
    medecinCombo = new QComboBox();
    medecinLabel->setBuddy(medecinCombo);

    // --- Patient ---
    patientLabel = new QLabel("&Patient :");
    patientCombo = new QComboBox();
    patientLabel->setBuddy(patientCombo);

    // --- Motif ---
    motifLabel = new QLabel("&Motif :");
    motifEdit = new QTextEdit();
    motifEdit->setMaximumHeight(80);
    motifLabel->setBuddy(motifEdit);

    // --- Ajout d'un médicament à la liste ---
    medicamentLabel = new QLabel("&Médicament :");
    medicamentCombo = new QComboBox();
    medicamentLabel->setBuddy(medicamentCombo);

    dureeLabel = new QLabel("Durée (jours) :");
    dureeSpin = new QSpinBox();
    dureeSpin->setRange(1, 365);
    dureeSpin->setValue(1);

    ajouterMedicamentButton = new QPushButton("Ajouter le médicament");

    // --- Tableau des médicaments prescrits ---
    medicamentsTable = new QTableWidget(0, 3);
    medicamentsTable->setHorizontalHeaderLabels({"Code", "Médicament", "Durée (jours)"});
    medicamentsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    medicamentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    medicamentsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    medicamentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    retirerMedicamentButton = new QPushButton("Retirer le médicament sélectionné");

    // --- Message d'erreur ---
    erreurLabel = new QLabel();
    erreurLabel->setStyleSheet("color: red;");
    erreurLabel->setWordWrap(true);

    // --- Bouton de validation ---
    valider = new QPushButton("Enregistrer la consultation");

    // =========================================
    // Disposition
    // =========================================
    int row = 0;
    gridLayout->addWidget(titre, row, 0, 1, 2);
    row++;

    gridLayout->addWidget(dateLabel, row, 0);
    gridLayout->addWidget(dateEdit, row, 1);
    row++;

    gridLayout->addWidget(medecinLabel, row, 0);
    gridLayout->addWidget(medecinCombo, row, 1);
    row++;

    gridLayout->addWidget(patientLabel, row, 0);
    gridLayout->addWidget(patientCombo, row, 1);
    row++;

    gridLayout->addWidget(motifLabel, row, 0);
    gridLayout->addWidget(motifEdit, row, 1);
    row++;

    gridLayout->addWidget(medicamentLabel, row, 0);
    gridLayout->addWidget(medicamentCombo, row, 1);
    row++;

    gridLayout->addWidget(dureeLabel, row, 0);
    gridLayout->addWidget(dureeSpin, row, 1);
    row++;

    gridLayout->addWidget(ajouterMedicamentButton, row, 0, 1, 2);
    row++;

    gridLayout->addWidget(medicamentsTable, row, 0, 1, 2);
    row++;

    gridLayout->addWidget(retirerMedicamentButton, row, 0, 1, 2);
    row++;

    gridLayout->addWidget(erreurLabel, row, 0, 1, 2);
    row++;

    gridLayout->addWidget(valider, row, 0, 1, 2);

    // =========================================
    // Connexion des boutons en slots
    // =========================================
    connect(
        ajouterMedicamentButton,
        &QPushButton::clicked,
        this,
        &ConsultationForm::ajouterMedicamentALaListe
        );

    connect(
        retirerMedicamentButton,
        &QPushButton::clicked,
        this,
        &ConsultationForm::retirerMedicamentDeLaListe
        );

    connect(
        valider,
        &QPushButton::clicked,
        this,
        &ConsultationForm::enregistrerConsultation
        );

    // =========================================
    // Chargement des données depuis la base
    // =========================================
    chargerMedecins();
    chargerPatients();
    chargerMedicaments();

    // Transformation des combos en barres de recherche (on tape, la liste
    // se filtre, on clique sur l'élément voulu dans la liste déroulante).
    configurerRecherche(medecinCombo, "Rechercher un médecin...");
    configurerRecherche(patientCombo, "Rechercher un patient...");
    configurerRecherche(medicamentCombo, "Rechercher un médicament...");
}

// ---------------------------------------------------------------------------
// Recharge les 3 listes déroulantes (à appeler à chaque ouverture de la page)
// ---------------------------------------------------------------------------
void ConsultationForm::actualiserListes()
{
    chargerMedecins();
    chargerPatients();
    chargerMedicaments();

    // On reconfigure la recherche, car chargerX() vide puis
    // reremplit le combo, ce qui peut affecter le completer.
    configurerRecherche(medecinCombo, "Rechercher un médecin...");
    configurerRecherche(patientCombo, "Rechercher un patient...");
    configurerRecherche(medicamentCombo, "Rechercher un médicament...");
}

// ---------------------------------------------------------------------------
// Utilitaire : trouve la vraie colonne parmi une liste de noms candidats
// ---------------------------------------------------------------------------
QString ConsultationForm::colonneCorrespondante(const QSqlRecord &rec, const QStringList &candidats)
{
    // 1) correspondance exacte (insensible à la casse)
    for (const QString &c : candidats)
    {
        int idx = rec.indexOf(c);
        if (idx >= 0)
            return rec.fieldName(idx);
    }
    // 2) correspondance partielle (le nom de colonne contient le mot-clé)
    for (int i = 0; i < rec.count(); ++i)
    {
        const QString champ = rec.fieldName(i).toLower();
        for (const QString &c : candidats)
        {
            if (champ.contains(c.toLower()))
                return rec.fieldName(i);
        }
    }
    return QString();
}

// ---------------------------------------------------------------------------
// Transforme un QComboBox en barre de recherche
// ---------------------------------------------------------------------------
void ConsultationForm::configurerRecherche(QComboBox *combo, const QString &texteIndicatif)
{
    // Rend le combo éditable : on peut taper du texte comme dans une barre
    // de recherche, sans pour autant pouvoir créer de nouvel élément.
    combo->setEditable(true);
    combo->setInsertPolicy(QComboBox::NoInsert);
    combo->setCurrentIndex(-1);
    combo->lineEdit()->clear();
    combo->lineEdit()->setPlaceholderText(texteIndicatif);

    // Le complétion utilise le même modèle que le combo : sélectionner une
    // suggestion dans la liste déroulante met bien à jour currentIndex()/
    // currentData(), pas seulement le texte affiché.
    QCompleter *completer = new QCompleter(combo->model(), combo);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    combo->setCompleter(completer);
}

// ---------------------------------------------------------------------------
// Chargement des listes déroulantes
// ---------------------------------------------------------------------------
void ConsultationForm::chargerMedecins()
{
    medecinCombo->clear();

    QSqlDatabase db = QSqlDatabase::database(CONNEXION_BD);
    if (!db.isOpen())
    {
        erreurLabel->setText("La connexion à la base de données est fermée.");
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT * FROM medecin"))
    {
        erreurLabel->setText("Impossible de charger les médecins.\n" + query.lastError().text());
        qDebug() << query.lastError().text();
        return;
    }

    QSqlRecord rec = query.record();
    QString colMatricule = colonneCorrespondante(rec, {"Matricule"});
    QString colNom       = colonneCorrespondante(rec, {"Nom"});

    if (colMatricule.isEmpty())
    {
        erreurLabel->setText("Colonne Matricule introuvable dans la table medecin.");
        return;
    }

    while (query.next())
    {
        int matricule = query.value(colMatricule).toInt();
        QString nom = colNom.isEmpty() ? QString() : query.value(colNom).toString();

        QString libelle = QString("%1 (Mat. %2)").arg(nom).arg(matricule);
        medecinCombo->addItem(libelle, matricule);
    }

    if (medecinCombo->count() == 0)
        erreurLabel->setText("Aucun médecin trouvé. Ajoutez d'abord un médecin.");
}

void ConsultationForm::chargerPatients()
{
    patientCombo->clear();

    QSqlDatabase db = QSqlDatabase::database(CONNEXION_BD);
    if (!db.isOpen())
    {
        erreurLabel->setText("La connexion à la base de données est fermée.");
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT * FROM patient"))
    {
        erreurLabel->setText("Impossible de charger les patients.\n" + query.lastError().text());
        qDebug() << query.lastError().text();
        return;
    }

    QSqlRecord rec = query.record();
    // Le schéma de la table "consulte" référence "Patient_Num_ss", mais le
    // formulaire patient existant insère dans une colonne "Numero" : on
    // accepte les deux noms pour rester compatible avec votre table réelle.
    QString colId  = colonneCorrespondante(rec, {"Num_ss", "NumSS", "NSS", "Numero"});
    QString colNom = colonneCorrespondante(rec, {"Nom"});

    if (colId.isEmpty())
    {
        erreurLabel->setText("Colonne identifiant patient introuvable dans la table patient.");
        return;
    }

    while (query.next())
    {
        int idPatient = query.value(colId).toInt();
        QString nom = colNom.isEmpty() ? QString() : query.value(colNom).toString();

        QString libelle = QString("%1 (N° %2)").arg(nom).arg(idPatient);
        patientCombo->addItem(libelle, idPatient);
    }

    if (patientCombo->count() == 0)
        erreurLabel->setText("Aucun patient trouvé. Ajoutez d'abord un patient.");
}

void ConsultationForm::chargerMedicaments()
{
    medicamentCombo->clear();

    QSqlDatabase db = QSqlDatabase::database(CONNEXION_BD);
    if (!db.isOpen())
    {
        erreurLabel->setText("La connexion à la base de données est fermée.");
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT * FROM medicament"))
    {
        erreurLabel->setText("Impossible de charger les médicaments.\n" + query.lastError().text());
        qDebug() << query.lastError().text();
        return;
    }

    QSqlRecord rec = query.record();
    QString colCode = colonneCorrespondante(rec, {"Code"});
    QString colNom  = colonneCorrespondante(rec, {"libelle", "Libelle", "Designation"});

    if (colCode.isEmpty())
    {
        erreurLabel->setText("Colonne Code introuvable dans la table medicament.");
        return;
    }

    while (query.next())
    {
        QString code = query.value(colCode).toString();
        QString nom  = colNom.isEmpty() ? QString() : query.value(colNom).toString();

        QString libelle = nom.isEmpty() ? code : QString("%1 - %2").arg(code, nom);
        medicamentCombo->addItem(libelle, code);
    }

    if (medicamentCombo->count() == 0)
        erreurLabel->setText("Aucun médicament trouvé. Ajoutez d'abord un médicament.");
}

// ---------------------------------------------------------------------------
// Gestion de la liste des médicaments prescrits (tableau)
// ---------------------------------------------------------------------------
bool ConsultationForm::medicamentDejaDansListe(const QString &code) const
{
    for (int row = 0; row < medicamentsTable->rowCount(); ++row)
    {
        if (medicamentsTable->item(row, 0)->text() == code)
            return true;
    }
    return false;
}

void ConsultationForm::ajouterMedicamentALaListe()
{
    erreurLabel->clear();

    if (medicamentCombo->currentIndex() < 0)
    {
        erreurLabel->setText("Sélectionnez un médicament avant de l'ajouter.");
        return;
    }

    QString code = medicamentCombo->currentData().toString();
    QString libelleComplet = medicamentCombo->currentText();
    int nombreJours = dureeSpin->value();

    if (medicamentDejaDansListe(code))
    {
        erreurLabel->setText("Ce médicament est déjà dans la liste.");
        return;
    }

    if (nombreJours <= 0)
    {
        erreurLabel->setText("La durée doit être supérieure à 0 jour.");
        return;
    }

    int row = medicamentsTable->rowCount();
    medicamentsTable->insertRow(row);
    medicamentsTable->setItem(row, 0, new QTableWidgetItem(code));
    medicamentsTable->setItem(row, 1, new QTableWidgetItem(libelleComplet));
    medicamentsTable->setItem(row, 2, new QTableWidgetItem(QString::number(nombreJours)));

    dureeSpin->setValue(1);
}

void ConsultationForm::retirerMedicamentDeLaListe()
{
    int row = medicamentsTable->currentRow();
    if (row < 0)
    {
        erreurLabel->setText("Sélectionnez d'abord un médicament dans le tableau.");
        return;
    }
    medicamentsTable->removeRow(row);
    erreurLabel->clear();
}

// ---------------------------------------------------------------------------
// Accès base de données
// ---------------------------------------------------------------------------
bool ConsultationForm::consultationAColonneMotif() const
{
    QSqlDatabase db = QSqlDatabase::database(CONNEXION_BD);
    QSqlRecord rec = db.record("consultation");
    return !colonneCorrespondante(rec, {"motif"}).isEmpty();
}

bool ConsultationForm::assurerRelationMedecinPatient(int matricule, int numSS, QString &erreur)
{
    QSqlDatabase db = QSqlDatabase::database(CONNEXION_BD);

    QSqlQuery verif(db);
    verif.prepare("SELECT COUNT(*) FROM consulte WHERE Medecin_Matricule = :m AND Patient_Num_ss = :p");
    verif.bindValue(":m", matricule);
    verif.bindValue(":p", numSS);
    if (!verif.exec() || !verif.next())
    {
        erreur = "Erreur lors de la vérification de la relation médecin/patient.\n" + verif.lastError().text();
        return false;
    }

    if (verif.value(0).toInt() > 0)
        return true; // la relation existe déjà

    QSqlQuery insertion(db);
    insertion.prepare("INSERT INTO consulte (Medecin_Matricule, Patient_Num_ss) VALUES (:m, :p)");
    insertion.bindValue(":m", matricule);
    insertion.bindValue(":p", numSS);
    if (!insertion.exec())
    {
        erreur = "Erreur lors de la création de la relation médecin/patient.\n" + insertion.lastError().text();
        return false;
    }
    return true;
}

int ConsultationForm::prochainNumeroConsultation(bool &ok)
{
    QSqlQuery query(QSqlDatabase::database(CONNEXION_BD));
    ok = query.exec("SELECT COALESCE(MAX(Numero), 0) + 1 FROM consultation");
    if (ok && query.next())
        return query.value(0).toInt();
    ok = false;
    return -1;
}

// ---------------------------------------------------------------------------
// Enregistrement
// ---------------------------------------------------------------------------
void ConsultationForm::enregistrerConsultation()
{
    erreurLabel->clear();

    // =========================================
    // Contrôles
    // =========================================
    if (medecinCombo->count() == 0 || medecinCombo->currentIndex() < 0)
    {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un médecin.");
        return;
    }

    if (patientCombo->count() == 0 || patientCombo->currentIndex() < 0)
    {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un patient.");
        return;
    }

    if (!dateEdit->date().isValid())
    {
        QMessageBox::warning(this, "Erreur", "La date de consultation est invalide.");
        return;
    }

    if (dateEdit->date() > QDate::currentDate())
    {
        QMessageBox::warning(this, "Erreur", "La date de consultation ne peut pas être dans le futur.");
        return;
    }

    if (medicamentsTable->rowCount() == 0)
    {
        auto reponse = QMessageBox::question(
            this,
            "Aucun médicament",
            "Aucun médicament n'a été ajouté à la liste. "
            "Voulez-vous enregistrer la consultation quand même ?",
            QMessageBox::Yes | QMessageBox::No
            );
        if (reponse == QMessageBox::No)
            return;
    }

    // =========================================
    // Connexion
    // =========================================
    QSqlDatabase db = QSqlDatabase::database(CONNEXION_BD);
    if (!db.isOpen())
    {
        QMessageBox::critical(this, "Erreur", "La connexion à la base de données est fermée.");
        return;
    }

    int matricule = medecinCombo->currentData().toInt();
    int numSS = patientCombo->currentData().toInt();

    if (!db.transaction())
    {
        QMessageBox::critical(this, "Erreur",
                              "Impossible de démarrer la transaction.\n" + db.lastError().text());
        return;
    }

    // 1) S'assurer que le médecin et le patient sont liés (table consulte)
    QString erreur;
    if (!assurerRelationMedecinPatient(matricule, numSS, erreur))
    {
        db.rollback();
        QMessageBox::critical(this, "Erreur", erreur);
        return;
    }

    // 2) Déterminer le prochain numéro de consultation
    bool ok = false;
    int numero = prochainNumeroConsultation(ok);
    if (!ok)
    {
        db.rollback();
        QMessageBox::critical(this, "Erreur", "Impossible de générer le numéro de consultation.");
        return;
    }

    // 3) Insertion de la consultation (avec motif, colonne désormais ajoutée à la table)
    QSqlQuery insertConsultation(db);

    insertConsultation.prepare(
        "INSERT INTO consultation "
        "(Numero, date, motif, Medecin_has_Patient_Medecin_Matricule, Medecin_has_Patient_Patient_Num_ss) "
        "VALUES (:numero, :date, :motif, :matricule, :numss)"
        );

    insertConsultation.bindValue(":numero", numero);
    insertConsultation.bindValue(":date", dateEdit->date().toString("yyyy-MM-dd"));
    insertConsultation.bindValue(":motif", motifEdit->toPlainText().trimmed());
    insertConsultation.bindValue(":matricule", matricule);
    insertConsultation.bindValue(":numss", numSS);

    if (!insertConsultation.exec())
    {
        db.rollback();
        QMessageBox::critical(this, "Erreur",
                              "Impossible d'enregistrer la consultation.\n" + insertConsultation.lastError().text());
        qDebug() << insertConsultation.lastError().text();
        return;
    }

    // 4) Insertion des médicaments prescrits
    for (int row = 0; row < medicamentsTable->rowCount(); ++row)
    {
        QString code = medicamentsTable->item(row, 0)->text();
        QString jours = medicamentsTable->item(row, 2)->text();

        QSqlQuery insertPrescrit(db);
        insertPrescrit.prepare(
            "INSERT INTO prescrit (Medicament_Code, Consultation_Numero, nombre_de_jours) "
            "VALUES (:code, :numero, :jours)"
            );
        insertPrescrit.bindValue(":code", code);
        insertPrescrit.bindValue(":numero", numero);
        insertPrescrit.bindValue(":jours", jours);

        if (!insertPrescrit.exec())
        {
            db.rollback();
            QMessageBox::critical(this, "Erreur",
                                  "Impossible d'enregistrer le médicament '" + code + "'.\n"
                                      + insertPrescrit.lastError().text());
            qDebug() << insertPrescrit.lastError().text();
            return;
        }
    }

    if (!db.commit())
    {
        db.rollback();
        QMessageBox::critical(this, "Erreur",
                              "Erreur lors de la validation de la transaction.\n" + db.lastError().text());
        return;
    }

    // =========================================
    // Succès
    // =========================================
    QMessageBox::information(this, "Succès",
                             QString("Consultation n°%1 enregistrée avec succès.").arg(numero));

    reinitialiserFormulaire();
}

void ConsultationForm::reinitialiserFormulaire()
{
    dateEdit->setDate(QDate::currentDate());
    motifEdit->clear();
    medicamentsTable->setRowCount(0);
    erreurLabel->clear();

    // On recharge les combos au cas où de nouveaux médecins/patients/
    // médicaments auraient été ajoutés entre-temps.
    chargerMedecins();
    chargerPatients();
    chargerMedicaments();
}