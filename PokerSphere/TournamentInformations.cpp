#include "TournamentInformations.h"

#include "requetes.h"

TournamentInformations::TournamentInformations(std::shared_ptr<Tournament> tournament, QWidget *parent)
	: QWidget(parent)/*, m_networkAdapter(nullptr)*/, m_tournament(tournament)
{
	ui.setupUi(this);

	QStringList args = QStringList();
	args.append(QString::number(m_tournament->getId()));
	
	ui.prizepoolSpinBox->setValue(m_tournament->getBuyIn() * m_tournament->getNbSubscribes());
	ui.nbInscritsSpinBox->setValue(m_tournament->getNbSubscribes());
	//m_networkAdapter = new NetworkAdapter(SELECT_TOURNAMENT_INFORMATIONS,SELECT,args,this);
	//connect(m_networkAdapter, SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getInformations(QStringList, QList<QStringList>)));
}

TournamentInformations::~TournamentInformations()
{
}

void TournamentInformations::getInformations(QStringList headers, QList<QStringList> result)
{
	if (result.size() == 1)
	{
		for (int i = 0 ; i < result[0].size() ; i++)
		{
			if (headers[i] == PRIZEPOOL)
				ui.prizepoolSpinBox->setValue(result[0][i].toInt());
			else
				if (headers[i] == NB_SUBSCRIBES)
					ui.nbInscritsSpinBox->setValue(result[0][i].toInt());
		}
	}
}