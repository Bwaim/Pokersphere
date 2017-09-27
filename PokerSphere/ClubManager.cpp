#include "ClubManager.h"

#include "requetes.h"
#include "Transform.h"
#include "Parameter.h"
#include "FranchiseSelection.h"

#include <QEventLoop>
#include <QTimer>

//Initialisation du singleton à NULL
ClubManager *ClubManager::m_singleton = nullptr;
int ClubManager::m_franchise = -1;

ClubManager::ClubManager()
    :  QObject(), m_networkAdapter(nullptr), m_headers(), m_clubs(), m_clubsLoaded(false)
{
    QStringList args = QStringList();
    args.append(QString::number(m_franchise));

    m_networkAdapter.reset(new NetworkAdapter(SELECT_CLUB,SELECT,args));
    connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this
        ,SLOT(getClubs(QStringList, QList<QStringList>)));

}


ClubManager::~ClubManager(void)
{
}

ClubManager* ClubManager::getInstance(int franchise)
{
    if (!m_singleton || m_franchise != franchise)
    {
        kill();
        m_franchise = franchise;
        m_singleton = new ClubManager();
    }

    return m_singleton;
}

void ClubManager::kill()
{
    if (m_singleton)
    {
        delete m_singleton;
        m_singleton = nullptr;
    }
}

void ClubManager::getClubs(QStringList headers, QList<QStringList> result)
{
    m_clubs = Transform::toClubs(headers,result);
    m_headers = headers;
    m_clubsLoaded = true;
    emit isLoaded();
}

bool ClubManager::add(QString club, QWidget *caller)
{
    if (m_clubs.contains(club))
        return false;

    m_clubs.append(club);
    m_clubs.sort();

    QStringList args = QStringList();
    args.append(club);

    args.append(QString::number(m_franchise));

    m_networkAdapter.reset(new NetworkAdapter(CREATE_CLUB,INSERT,args));

    connect(m_networkAdapter.get(), SIGNAL(finished()), caller ,SLOT(accept()));

    return true;
}

QStringList ClubManager::getClubs()
{
    return m_clubs;
}

bool ClubManager::modify(QString clubNew, QString clubOld, QWidget *caller)
{
    if (clubOld.compare(clubNew) != 0
        && m_clubs.contains(clubNew))
        return false;

    m_clubs.removeAt(m_clubs.indexOf(clubOld));
    m_clubs.append(clubNew);
    m_clubs.sort();

    QStringList args = QStringList();

    args.append(clubOld);
    args.append(clubNew);

    args.append(QString::number(m_franchise));

    m_networkAdapter.reset(new NetworkAdapter(UPDATE_CLUB,UPDATE,args,caller));

    return true;
}

void ClubManager::remove(QString name, QWidget *caller)
{
    m_clubs.removeAt(m_clubs.indexOf(name));

    QStringList args = QStringList();
    args.append(name);

    args.append(QString::number(m_franchise));

    m_networkAdapter.reset(new NetworkAdapter(DELETE_CLUB,DELETE_REQ,args,caller));
}

QStringList ClubManager::getHeaders()
{
    return m_headers;
}

bool ClubManager::isClubsLoaded()
{
    return m_clubsLoaded;
}

void ClubManager::fillClubs(QComboBox *comboBox)
{
    QTimer timer;
    timer.setSingleShot(true);
    QEventLoop loop;

    connect(this,  SIGNAL(isLoaded()), &loop, SLOT(quit()) );
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer.start(1000);

    loop.exec();

    while (!m_clubsLoaded)
    {
        timer.start(1000);
        loop.exec();
    }

    comboBox->clear();
    comboBox->addItem(QString(""));
    comboBox->addItems(m_clubs);
}
