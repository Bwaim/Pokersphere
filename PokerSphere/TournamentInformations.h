#ifndef TOURNAMENTINFORMATIONS_H
#define TOURNAMENTINFORMATIONS_H

#include <QWidget>
#include "ui_TournamentInformations.h"
#include "networkadapter.h"
#include "Tournament.h"

class TournamentInformations : public QWidget
{
	Q_OBJECT

public:
	TournamentInformations(std::shared_ptr<Tournament> tournament, QWidget *parent = 0);
	~TournamentInformations();

private slots:
	void getInformations(QStringList headers, QList<QStringList> result);

private:
	Ui::TournamentInformationsClass ui;

	//std::unique_ptr<NetworkAdapter> m_networkAdapter;
	std::shared_ptr<Tournament> m_tournament;
};

#endif // TOURNAMENTINFORMATIONS_H
