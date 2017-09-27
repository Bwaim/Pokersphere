#ifndef CREATECLUB_H
#define CREATECLUB_H

#include <QDialog>

#include "ClubManager.h"

namespace Ui {
class CreateClub;
}

class CreateClub : public QDialog
{
    Q_OBJECT

public:
    CreateClub(int franchise, QWidget *parent = 0);
    CreateClub(QString club, int franchise, QWidget *parent = 0);
    ~CreateClub();

private slots:
    void cancelCreation(const QString&);
    void saveClub(const QString&);

private:
    void init(int franchise);

    Ui::CreateClub *ui;

    ClubManager *m_clubManager;
};

#endif // CREATECLUB_H
