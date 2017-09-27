#include "TournamentTemplate.h"


TournamentTemplate::TournamentTemplate(void)
{
}

TournamentTemplate::TournamentTemplate(TournamentTemplate const& tournamentTemplate)
	: m_id(tournamentTemplate.m_id), m_name(tournamentTemplate.m_name), m_buyIn(tournamentTemplate.m_buyIn)
    , m_nbPlayers(tournamentTemplate.m_nbPlayers), m_variante(tournamentTemplate.m_variante), m_private(tournamentTemplate.m_private)
    , m_comment(tournamentTemplate.m_comment), m_franchise(tournamentTemplate.m_franchise)
{

}

TournamentTemplate::TournamentTemplate(QString name, int buyIn, int nbPlayers, int variante, QString comment)
    : m_name(name), m_buyIn(buyIn), m_nbPlayers(nbPlayers), m_variante(variante), m_comment(comment)
{

}

TournamentTemplate::~TournamentTemplate(void)
{
}

TournamentTemplate& TournamentTemplate::operator=(TournamentTemplate const& tournamentTemplate)
{
	if (this != &tournamentTemplate)
	{
		m_id = tournamentTemplate.m_id;
		m_name = tournamentTemplate.m_name;
		m_buyIn = tournamentTemplate.m_buyIn;
		m_nbPlayers = tournamentTemplate.m_nbPlayers;
		m_variante = tournamentTemplate.m_variante;
        m_private = tournamentTemplate.m_private;
        m_comment = tournamentTemplate.m_comment;
        m_franchise = tournamentTemplate.m_franchise;
	}

	return *this;
}

int TournamentTemplate::getId() const
{
	return m_id;
}

void TournamentTemplate::setId(int id)
{
	m_id = id;
}

QString TournamentTemplate::getName() const
{
	return m_name;
}

void TournamentTemplate::setName(QString name)
{
	m_name = name;
}

int TournamentTemplate::getBuyIn() const
{
	return m_buyIn;
}

void TournamentTemplate::setBuyIn(int buyIn)
{
	m_buyIn = buyIn;
}

int TournamentTemplate::getNbPlayers() const
{
	return m_nbPlayers;
}

void TournamentTemplate::setNbPlayers(int nbPlayers)
{
	m_nbPlayers = nbPlayers;
}

int TournamentTemplate::getVariante() const
{
	return m_variante;
}

void TournamentTemplate::setVariante(int variante)
{
	m_variante = variante;
}

bool TournamentTemplate::getPrivate(void) const
{
    return m_private;
}


void TournamentTemplate::setPrivate(bool privat)
{
    m_private = privat;
}

QString TournamentTemplate::getComment() const
{
	return m_comment;
}

void TournamentTemplate::setComment(QString comment)
{
	m_comment = comment;
}

void TournamentTemplate::setFranchise(int franchise)
{
    m_franchise = franchise;
}

int TournamentTemplate::getFranchise() const
{
    return m_franchise;
}
