#include "Tournament.h"


Tournament::Tournament(void)
    : TournamentTemplate(), m_isSentToRankingHero(false)
{
}

Tournament::Tournament(Tournament const& tournament)
	: TournamentTemplate(tournament), m_beginDate(tournament.m_beginDate), m_endRegistrationDate(tournament.m_endRegistrationDate)
    , m_nbSubscribes(tournament.m_nbSubscribes), m_isValidated(tournament.m_isValidated), m_isSentToRankingHero(tournament.m_isSentToRankingHero)
    , m_attachedTournament(tournament.m_attachedTournament)
{

}

Tournament::~Tournament(void)
{
}

Tournament& Tournament::operator=(Tournament const& tournament)
{
	if (this != &tournament)
	{
		m_id = tournament.m_id;
		m_name = tournament.m_name;
		m_beginDate = tournament.m_beginDate;
		m_endRegistrationDate = tournament.m_endRegistrationDate;
		m_buyIn = tournament.m_buyIn;
		m_nbPlayers = tournament.m_nbPlayers;
		m_variante = tournament.m_variante;
		m_nbSubscribes = tournament.m_nbSubscribes;
		m_isValidated = tournament.m_isValidated;
		m_comment = tournament.m_comment;
        m_isSentToRankingHero = tournament.m_isSentToRankingHero;
        m_attachedTournament = tournament.m_attachedTournament;
	}

	return *this;
}

QDateTime Tournament::getBeginDate() const
{
	return m_beginDate;
}

void Tournament::setBeginDate(QDateTime beginDate)
{
	m_beginDate = beginDate;
}

QDateTime Tournament::getEndRegistrationDate() const
{
	return m_endRegistrationDate;
}

void Tournament::setEndRegistrationDate(QDateTime endRegistrationDate)
{
	m_endRegistrationDate = endRegistrationDate;
}

int Tournament::getNbSubscribes() const
{
	return m_nbSubscribes;
}

void Tournament::setNbSubscribes(int nbSubscribes)
{
	m_nbSubscribes = nbSubscribes;
}

bool Tournament::getIsValidated() const
{
	return m_isValidated;
}

void Tournament::setIsValidated(bool isValidated)
{
	m_isValidated = isValidated;
}

bool Tournament::getIsSentToRankingHero() const
{
    return m_isSentToRankingHero;
}

void Tournament::setIsSentToRankingHero(bool isSentToRankingHero)
{
    m_isSentToRankingHero = isSentToRankingHero;
}

int Tournament::getAttachedTournament() const
{
    return m_attachedTournament;
}

void Tournament::setAttachedTournament(int tournament)
{
    m_attachedTournament = tournament;
}
