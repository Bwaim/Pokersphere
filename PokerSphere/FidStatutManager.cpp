#include "FidStatutManager.h"

#include "requetes.h"
#include "Transform.h"

#include <QStringList>

//Initialisation du singleton à NULL
FidStatutManager *FidStatutManager::m_singleton = nullptr;

FidStatutManager::FidStatutManager()
    : QObject(), m_networkAdapter(nullptr), m_fidStatuts(), m_headers(), m_fidStatutAdded(nullptr)
{
    QStringList args = QStringList();

    m_networkAdapter.reset(new NetworkAdapter(SELECT_FID_STATUTS,SELECT,args));
    connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this
        ,SLOT(getFidStatuts(QStringList,QList<QStringList>)));
}

FidStatutManager::~FidStatutManager(void)
{
    if (m_fidStatuts.count() > 0)
    {
        QMap<QString, std::shared_ptr<FidStatut>>::iterator i = m_fidStatuts.begin();
        while (i != m_fidStatuts.end())
        {
            i = m_fidStatuts.erase(i);
        }
    }
}

FidStatutManager* FidStatutManager::getInstance()
{
    if (!m_singleton)
    {
        kill();
        m_singleton = new FidStatutManager();
    }

    return m_singleton;
}

void FidStatutManager::kill()
{
    if (m_singleton)
    {
        delete m_singleton;
        m_singleton = nullptr;
    }
}

void FidStatutManager::getFidStatuts(QStringList headers, QList<QStringList> result)
{
    m_fidStatuts = Transform::toFidStatuts(headers,result);
    m_headers = headers;
    emit isLoaded();
}

bool FidStatutManager::add(std::shared_ptr<FidStatut> fidStatut, QWidget *caller)
{
    if (m_fidStatuts.contains(fidStatut->getName()))
        return false;

    m_fidStatuts.insert(fidStatut->getName(),fidStatut);

    QStringList args = QStringList();
    args.append(fidStatut->getName());
    args.append(QString::number(fidStatut->getSeuil()));

    m_networkAdapter.reset(new NetworkAdapter(CREATE_FID_STATUT,INSERT,args));

    connect(m_networkAdapter.get(), SIGNAL(finished()), caller ,SLOT(accept()));
    connect(m_networkAdapter.get(), SIGNAL(finished(int)), this ,SLOT(fillId(int)));

    return true;
}

QStringList FidStatutManager::getHeaders()
{
    return m_headers;
}

QMap<QString, std::shared_ptr<FidStatut>> FidStatutManager::getFidStatuts()
{
    return m_fidStatuts;
}

std::shared_ptr<FidStatut> FidStatutManager::get(QString name)
{
    return m_fidStatuts.value(name);
}

bool FidStatutManager::modify(FidStatut &fidStatutNew, std::shared_ptr<FidStatut> fidStatutOld, QWidget *caller)
{
    if (fidStatutOld->getName().compare(fidStatutNew.getName()) != 0
        && m_fidStatuts.contains(fidStatutNew.getName()))
        return false;

    m_fidStatuts.remove(fidStatutOld->getName());
    fidStatutOld->setName(fidStatutNew.getName());
    fidStatutOld->setSeuil(fidStatutNew.getSeuil());
    m_fidStatuts.insert(fidStatutOld->getName(), fidStatutOld);

    QStringList args = QStringList();

    args.append(QString::number(fidStatutOld->getId()));
    args.append(fidStatutNew.getName());
    args.append(QString::number(fidStatutNew.getSeuil()));

    m_networkAdapter.reset(new NetworkAdapter(UPDATE_FID_STATUT,UPDATE,args,caller));

    return true;
}

void FidStatutManager::remove(QString name, QWidget *caller)
{
    std::shared_ptr<FidStatut> fidStatut = m_fidStatuts.take(name);

    QStringList args = QStringList();
    args.append(QString::number(fidStatut->getId()));

    m_networkAdapter.reset(new NetworkAdapter(DELETE_FID_STATUT,DELETE_REQ,args,caller));
}

void FidStatutManager::setLastFidSatutAdded(std::shared_ptr<FidStatut> fidStatutAdded)
{
    m_fidStatutAdded = fidStatutAdded;
}

void FidStatutManager::fillId(int id)
{
    m_fidStatutAdded->setId(id);
}
