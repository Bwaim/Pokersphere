#ifndef MEMBERSHIP_H
#define MEMBERSHIP_H

#include <QDialog>
#include <QHash>
#include "ui_membership.h"
#include "Member.h"
#include "Adhesion.h"
#include "networkadapter.h"
#include "createadhesion.h"
#include "MembershipManager.h"

class Membership : public QDialog
{
	Q_OBJECT

public:
	Membership(std::shared_ptr<Member> member, QWidget *parent = 0);
	~Membership();

private slots:
	void getMemberships(QStringList headers, QList<QStringList> result);
	void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
	void showCreateAdhesion(const QString&);
	void updateAdhesion(const QString&);
	void deleteAdhesion(const QString&);
	void typeAbonnementChanged(int index);


private:
	void displayAdhesion(std::shared_ptr<Adhesion> a);

	Ui::MembershipClass ui;

	std::shared_ptr<Member> m_member;
    std::shared_ptr<Adhesion> m_selectedAdhesion;
    std::unique_ptr<NetworkAdapter> m_networkAdapter;
	QHash<int,std::shared_ptr<Adhesion>> m_adhesion;
	std::unique_ptr<CreateAdhesion> m_createAdhesion;
    MembershipManager *m_membershipManager;
};

#endif // MEMBERSHIP_H
