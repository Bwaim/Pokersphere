#ifndef CLUB_MANAGER_H
#define CLUB_MANAGER_H

#include <QObject>
#include <QComboBox>

#include "networkadapter.h"

class ClubManager : public QObject
{
    Q_OBJECT

private:
    ClubManager();
    ~ClubManager();

signals:
    void isLoaded(void);

public:
    static ClubManager* getInstance(int franchise);
    static void kill();
    bool add(QString club, QWidget *caller);
    bool modify(QString clubNew, QString clubOld, QWidget *caller);
    void remove(QString name, QWidget *caller);
    QStringList getHeaders();
    QStringList getClubs();
    bool isClubsLoaded();
    void fillClubs(QComboBox *comboBox);

private slots:
    void getClubs(QStringList headers, QList<QStringList> result);

private:
    static ClubManager *m_singleton;
    static int m_franchise;

    std::unique_ptr<NetworkAdapter> m_networkAdapter;
    QStringList m_headers;
    QStringList m_clubs;
    bool m_clubsLoaded;
};

#endif // CLUB_MANAGER_H
