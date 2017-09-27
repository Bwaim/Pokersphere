#ifndef TOURNAMENTDETAIL_H
#define TOURNAMENTDETAIL_H

#include <QWidget>
#include <QSet>
#include <QFile>
#include <QMap>
#include "ui_TournamentDetail.h"
#include "Tournament.h"
#include "Member.h"
#include "networkadapter.h"
#include "membership.h"
#include "TournamentSubscribe.h"
#include "MemberElimination.h"
#include "MultipleSubscriptionSelection.h"

class TournamentDetail : public QWidget
{
	Q_OBJECT

public:
	TournamentDetail(std::shared_ptr<Tournament> tournament, QWidget *parent = 0);
	~TournamentDetail();

signals:
	void setPresent(bool);

private slots:
	void getMembers(QStringList headers, QList<QStringList> result);
	void unsubscribe(int id);
	void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
	void showMembership(const QString&);
    void showKosmosHistoric(const QString&);
	void showTournamentSubscribe(const QString&);
	void startTournament(const QString&);
	void setPresent(int numLine);
    void updatePhoto(QStringList infos, QString);
	void showMemberElimination(int id);
	void extractRanking(QString);
	void extractRanking(QStringList headers, QList<QStringList> result);
    void doubleClicked(const QModelIndex &current);
    void updateList();
    void extractMails(QStringList headers, QList<QStringList> result);
	void extractMails(const QString&);
    void sendRankingHero(QString);
    void importSubscription(QString);
    void showResultControl(QString);
    void finalizeTournament(QString);

private:
	Ui::TournamentDetailClass ui;

	void writeExportCSV(QFile &file, QStringList headers, QList<QStringList> result);
    void subscribeByEmail(QString email, QString name, QString firstname);

	std::shared_ptr<Tournament> m_tournament;
	std::unique_ptr<NetworkAdapter> m_networkAdapter;
    QHash<int,std::shared_ptr<Member>> m_members;
	std::unique_ptr<Membership> m_membership;
    std::unique_ptr<TournamentSubscribe> m_tournamentSubscribe;
    std::shared_ptr<Member> m_selectedMember;
    QSet<int> m_presents;
    std::unique_ptr<NetworkAdapter> m_networkAdapterPhoto;
    std::unique_ptr<MultipleSubscriptionSelection> m_multipleSubscriptionSelection;
    bool m_hasOut;
	int m_nbIn;
	QList<int> m_membersOut;
	std::unique_ptr<MemberElimination> m_memberElimination;
    bool m_allowPresent;
    std::shared_ptr<QMap<int, bool>> m_positions;
    	
};

#endif // TOURNAMENTDETAIL_H
