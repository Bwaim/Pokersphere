#ifndef MEMBERSHIPMANAGEMENT_H
#define MEMBERSHIPMANAGEMENT_H

#include <QWidget>
#include <QHash>
#include <QModelIndex>
#include "ui_MembershipManagement.h"
#include "networkadapter.h"
#include "Member.h"
#include "createmembership.h"
#include "membership.h"
#include "PhotoChange.h"
#include "EurosLoading.h"
#include "CreateClub.h"

class MembershipManagement : public QWidget
{
	Q_OBJECT

public:
	MembershipManagement(QWidget *parent = 0);
	~MembershipManagement();

private slots:
	void getMembers(QStringList headers, QList<QStringList> result);
	void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
	void showCreateMembership(const QString&);
	void showMembership(const QString&);
	void updateMembership(const QString&);
	void deleteMembership(const QString&);
    void updatePhoto(QStringList infos, QString);
	void showPhotoChange(const QString&);
    void doubleClicked(const QModelIndex &current);
    void chooseSponsor(bool);
    void updateSponsor();
    void feedFranchise(QStringList headers, QList<QStringList> result);
    void doFilter(bool);
    void changeFranchise(const QString&);
    void showEurosLoading(const QString&);
    void showAddClub(bool);

private:
	void displayMember(std::shared_ptr<Member> m, int abonnement);
    bool cardNumberIsUse(QString cardNumber);
    void loadClubs(int franchise);

	Ui::MembershipManagementClass ui;

    std::unique_ptr<CreateMembership> m_createMembership;
    std::unique_ptr<Membership> m_membership;
    std::shared_ptr<Member> m_selectedMember;
    QHash<int,std::shared_ptr<Member>> m_member;
    std::unique_ptr<PhotoChange> m_photoChange;
    std::unique_ptr<EurosLoading> m_eurosLoading;
    std::unique_ptr<NetworkAdapter> m_networkAdapter;
	std::unique_ptr<NetworkAdapter> m_networkAdapterPhoto;
    std::unique_ptr<NetworkAdapter> m_networkAdapterFranchise;
    std::unique_ptr<CreateClub> m_createClub;
};

#endif // MEMBERSHIPMANAGEMENT_H
