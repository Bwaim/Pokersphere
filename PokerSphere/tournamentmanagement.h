#ifndef TOURNAMENTMANAGEMENT_H
#define TOURNAMENTMANAGEMENT_H

#include <QWidget>
#include <QHash>
#include <QModelIndex>
#include "ui_tournamentmanagement.h"
#include "networkadapter.h"
#include "Tournament.h"
#include "createtournament.h"
#include "TournamentDetail.h"
#include "TournamentInformations.h"

class TournamentManagement : public QWidget
{
	Q_OBJECT

public:
	TournamentManagement(int typeTournament, QWidget *parent = 0);
	~TournamentManagement();

    QWidget* getMainWindow();

public slots:
	void doFilter(bool);

private slots:
	void getTournaments(QStringList headers, QList<QStringList> result);
	void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
	void showCreateTournament(const QString&);
	void updateTournament(const QString&);
	void deleteTournament(const QString&);
	void showTournamentSubscribes(const QString&);
	void showTournamentInformations(const QString&);
    void duplicateTournament(const QString&);
    void doubleClicked(const QModelIndex &current);
    void feedFranchise(QStringList headers, QList<QStringList> result);
    void feedTournamentList(QStringList headers, QList<QStringList> result);

private:
	void displayTournament(std::shared_ptr<Tournament> m);

	Ui::TournamentManagementClass ui;

    std::unique_ptr<CreateTournament> m_createTournament;
    std::shared_ptr<Tournament> m_selectedTournament;
    std::unique_ptr<NetworkAdapter> m_networkAdapter;
    std::unique_ptr<TournamentInformations> m_tournamentInformations;
    bool m_presentValidated;
    int m_typeTournament;
    QWidget *m_mainWindow;
    std::unique_ptr<NetworkAdapter> m_networkAdapterFranchise;
    std::unique_ptr<NetworkAdapter> m_networkAdapterTournamentList;
	QHash<int,std::shared_ptr<Tournament>> m_tournament;
};

#endif // TOURNAMENTMANAGEMENT_H
