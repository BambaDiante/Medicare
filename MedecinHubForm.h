#ifndef MEDECINHUBFORM_H
#define MEDECINHUBFORM_H

#include <QWidget>

class MedecinForm;
class MedecinListeForm;

class MedecinHubForm : public QWidget
{
    Q_OBJECT

public:

    explicit MedecinHubForm(QWidget *parent = nullptr);

private:

    MedecinForm *medecinForm;

    MedecinListeForm *medecinListeForm;
};

#endif // MEDECINHUBFORM_H