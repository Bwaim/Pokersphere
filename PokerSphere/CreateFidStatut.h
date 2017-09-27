#ifndef CREATEFIDSTATUT_H
#define CREATEFIDSTATUT_H

#include <QDialog>
#include <QString>
#include "FidStatutManager.h"

namespace Ui {
class CreateFidStatut;
}

class CreateFidStatut : public QDialog
{
    Q_OBJECT

public:
    CreateFidStatut(QWidget *parent = 0);
    CreateFidStatut(QString name, int seuil, QWidget *parent = 0);
    ~CreateFidStatut();

private slots:
    void cancelCreation(const QString&);
    void saveFidStatut(const QString&);

private:
    void init();

    Ui::CreateFidStatut *ui;

    FidStatutManager *m_fidStatutManager;
};

#endif // CREATEFIDSTATUT_H
