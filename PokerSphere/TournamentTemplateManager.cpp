#include "TournamentTemplateManager.h"

#include "requetes.h"
#include "Transform.h"
#include "Parameter.h"
#include "FranchiseSelection.h"

#include <QStringList>

//Initialisation du singleton à NULL
TournamentTemplateManager *TournamentTemplateManager::m_singleton = nullptr;
int TournamentTemplateManager::m_franchise = -1;

TournamentTemplateManager::TournamentTemplateManager()
    :  QObject(), m_networkAdapter(nullptr), m_tournamentsTemplate(), m_headers(), m_templateAdded(nullptr)
{
    QStringList args = QStringList();
    args.append(QString::number(m_franchise));

    m_networkAdapter.reset(new NetworkAdapter(SELECT_TOURNAMENT_TEMPLATE,SELECT,args));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this 
        ,SLOT(getTournamentsTemplate(QStringList, QList<QStringList>)));

}


TournamentTemplateManager::~TournamentTemplateManager(void)
{
    if (m_tournamentsTemplate.count() > 0)
	{
		QMap<QString, std::shared_ptr<TournamentTemplate>>::iterator i = m_tournamentsTemplate.begin();
		while (i != m_tournamentsTemplate.end())
		{
			//delete i.value();
			i = m_tournamentsTemplate.erase(i);
		}
	}
}

TournamentTemplateManager* TournamentTemplateManager::getInstance(int franchise)
{
    if (!m_singleton || m_franchise != franchise)
	{
        kill();
        m_franchise = franchise;
		m_singleton = new TournamentTemplateManager();
	}

	return m_singleton;
}

void TournamentTemplateManager::kill()
{
	if (m_singleton)
	{
		delete m_singleton;
		m_singleton = nullptr;
	}
}

void TournamentTemplateManager::getTournamentsTemplate(QStringList headers, QList<QStringList> result)
{
	m_tournamentsTemplate = Transform::toTournamentsTemplate(headers,result);
	m_headers = headers;
    emit isLoaded();
}

bool TournamentTemplateManager::add(std::shared_ptr<TournamentTemplate> tournamentTemplate, QWidget *caller)
{
    if (m_tournamentsTemplate.contains(tournamentTemplate->getName()))
        return false;

    m_tournamentsTemplate.insert(tournamentTemplate->getName(),tournamentTemplate);

    QStringList args = QStringList();
	args.append(tournamentTemplate->getName());
	args.append(QString::number(tournamentTemplate->getBuyIn()));
	args.append(QString::number(tournamentTemplate->getNbPlayers()));
	args.append(QString::number(tournamentTemplate->getVariante()));
    args.append(tournamentTemplate->getComment());

    Parameter *param = Parameter::getInstance();
    int franchise = param->getFranchise();
    if (franchise == 0)
    {
        FranchiseSelection franchiseSelection(&franchise);
        franchiseSelection.exec();
    }
    args.append(QString::number(franchise));
	
    m_networkAdapter.reset(new NetworkAdapter(CREATE_TOURNAMENT_TEMPLATE,INSERT,args));
    
	connect(m_networkAdapter.get(), SIGNAL(finished()), caller ,SLOT(accept()));
    connect(m_networkAdapter.get(), SIGNAL(finished(int)), this ,SLOT(fillId(int)));

    return true;
}

QStringList TournamentTemplateManager::getHeaders()
{
    return m_headers;
}

QMap<QString, std::shared_ptr<TournamentTemplate>> TournamentTemplateManager::getTemplates()
{
    return m_tournamentsTemplate;
}

std::shared_ptr<TournamentTemplate> TournamentTemplateManager::get(QString name)
{
    return m_tournamentsTemplate.value(name);
}

bool TournamentTemplateManager::modify(TournamentTemplate &tournamentTemplateNew, std::shared_ptr<TournamentTemplate> tournamentTemplateOld, QWidget *caller)
{
    if (tournamentTemplateOld->getName().compare(tournamentTemplateNew.getName()) != 0
        && m_tournamentsTemplate.contains(tournamentTemplateNew.getName()))
        return false;

    m_tournamentsTemplate.remove(tournamentTemplateOld->getName());
    tournamentTemplateOld->setName(tournamentTemplateNew.getName());
    tournamentTemplateOld->setBuyIn(tournamentTemplateNew.getBuyIn());
    tournamentTemplateOld->setNbPlayers(tournamentTemplateNew.getNbPlayers());
    tournamentTemplateOld->setVariante(tournamentTemplateNew.getVariante());
    tournamentTemplateOld->setComment(tournamentTemplateNew.getComment());
    m_tournamentsTemplate.insert(tournamentTemplateOld->getName(), tournamentTemplateOld);

    QStringList args = QStringList();
    
    args.append(QString::number(tournamentTemplateOld->getId()));
    args.append(tournamentTemplateNew.getName());
    args.append(QString::number(tournamentTemplateNew.getBuyIn()));
    args.append(QString::number(tournamentTemplateNew.getNbPlayers()));
    args.append(QString::number(tournamentTemplateNew.getVariante()));
    args.append(tournamentTemplateNew.getComment());
    
    m_networkAdapter.reset(new NetworkAdapter(UPDATE_TOURNAMENT_TEMPLATE,UPDATE,args,caller));

    return true;
}

void TournamentTemplateManager::remove(QString name, QWidget *caller)
{
    std::shared_ptr<TournamentTemplate> tournamentTemplate = m_tournamentsTemplate.take(name);
    
    QStringList args = QStringList();
    args.append(QString::number(tournamentTemplate->getId()));

    m_networkAdapter.reset(new NetworkAdapter(DELETE_TOURNAMENT_TEMPLATE,DELETE_REQ,args,caller));
}

void TournamentTemplateManager::setLastTemplateAdded(std::shared_ptr<TournamentTemplate> templateAdded)
{
    m_templateAdded = templateAdded;
}

void TournamentTemplateManager::fillId(int id)
{
    m_templateAdded->setId(id);
}