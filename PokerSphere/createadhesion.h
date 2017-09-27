#ifndef CREATEADHESION_H
#define CREATEADHESION_H

#include <QDialog>
#include <QDate>
#include "ui_createadhesion.h"
#include "Member.h"
#include "networkadapter.h"
#include "MembershipManager.h"

class CreateAdhesion : public QDialog
{
	Q_OBJECT

public:
	CreateAdhesion(std::shared_ptr<Member> member, QDate endDate, QWidget *parent = 0);
	~CreateAdhesion();

private slots:
	void groupChanged(int state);
	void typeAbonnementChanged(int index);
	void beginDateChanged(const QDate &date);
	void cancelCreation(const QString&);
	void saveCreation(const QString&);

private:
	Ui::CreateAdhesionClass ui;

	std::shared_ptr<Member> m_member;
	QDate m_endDate;
	std::unique_ptr<NetworkAdapter> m_networkAdapter;
	MembershipManager *m_membershipManager;
};

#endif // CREATEADHESION_H
