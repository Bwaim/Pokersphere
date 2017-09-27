#ifndef CLUBMANAGEMENT_H
#define CLUBMANAGEMENT_H

#include <QWidget>
#include "ClubManager.h"
#include "CreateClub.h"

namespace Ui {
class ClubManagement;
}

class ClubManagement : public QWidget
{
    Q_OBJECT

public:
    explicit ClubManagement(int franchise, QWidget *parent = 0);
    ~ClubManagement();

private slots:
    void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void showCreateClub(const QString&);
    void updateClub(const QString&);
    void deleteClub(const QString&);
    void getClubs();

private:
    void displayClub(QString club);

    Ui::ClubManagement *ui;

    ClubManager *m_clubManager;
    std::unique_ptr<CreateClub> m_createClub;
    QString m_selectedClub;
    int m_franchise;
};

#endif // CLUBMANAGEMENT_H
