#ifndef MEDICAMENTHUBFORM_H
#define MEDICAMENTHUBFORM_H

#include <QWidget>

class MedicamentForm;
class MedicamentListeForm;

class MedicamentHubForm : public QWidget
{
    Q_OBJECT

public:

    explicit MedicamentHubForm(QWidget *parent = nullptr);

private:

    MedicamentForm *medicamentForm;

    MedicamentListeForm *medicamentListeForm;
};

#endif // MEDICAMENTHUBFORM_H