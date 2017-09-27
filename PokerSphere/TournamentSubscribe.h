#ifndef TOURNAMENTSUBSCRIBE_H
#define TOURNAMENTSUBSCRIBE_H

#include <QDialog>
#include "ui_TournamentSubscribe.h"
#include "Member.h"
#include "networkadapter.h"
#include "membership.h"
#include "Tournament.h"

class TournamentSubscribe : public QDialog
{
	Q_OBJECT

public:
	TournamentSubscribe(std::shared_ptr<Tournament> tournament, QWidget *parent = 0);
	~TournamentSubscribe();

private slots:
	void getMembers(QStringList headers, QList<QStringList> result);
	void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
	void showMembership(const QString&);
	void doFilter(bool);
    void doFilterFranchise(bool);
	void subscribe(const QString&);
    void updatePhoto(QStringList infos, QString);
    void doubleClicked(const QModelIndex &current);

private:
	Ui::TournamentSubscribeClass ui;

    std::shared_ptr<Tournament> m_tournament;
	std::unique_ptr<NetworkAdapter> m_networkAdapter;
    std::unique_ptr<Membership> m_membership;
    std::shared_ptr<Member> m_selectedMember;
    std::unique_ptr<NetworkAdapter> m_networkAdapterPhoto;
	QHash<int,std::shared_ptr<Member>> m_members;
};

#endif // TOURNAMENTSUBSCRIBE_H
