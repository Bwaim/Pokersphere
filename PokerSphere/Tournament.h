#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include <QString>
#include <QDateTime>

#include "TournamentTemplate.h"

class Tournament : public TournamentTemplate
{
public:
	Tournament(void);
	Tournament(Tournament const& tournament);
	~Tournament(void);

	Tournament& operator=(Tournament const& tournament);

	QDateTime getBeginDate() const;
	void setBeginDate(QDateTime beginDate);
	QDateTime getEndRegistrationDate() const;
	void setEndRegistrationDate(QDateTime endRegistrationDate);
	int getNbSubscribes() const;
	void setNbSubscribes(int nbSubscribes);
	bool getIsValidated() const;
	void setIsValidated(bool isValidated);
    bool getIsSentToRankingHero() const;
	void setIsSentToRankingHero(bool isSentToRankingHero);
    int getAttachedTournament() const;
	void setAttachedTournament(int tournament);

private:
	QDateTime m_beginDate;
	QDateTime m_endRegistrationDate;
	int m_nbSubscribes;
	bool m_isValidated;
    bool m_isSentToRankingHero;
    int m_attachedTournament;
};

#endif // TOURNAMENT_H