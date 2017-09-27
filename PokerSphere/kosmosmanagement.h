#ifndef KOSMOSMANAGEMENT_H
#define KOSMOSMANAGEMENT_H

#include <QWidget>
#include <QHash>
#include "ui_kosmosmanagement.h"
#include "networkadapter.h"
#include "Member.h"
#include "retraitkosmos.h"
#include "depotkosmos.h"
#include "createenchere.h"
#include "kosmoshistoric.h"

class KosmosManagement : public QWidget
{
	Q_OBJECT

public:
	KosmosManagement(QWidget *parent = 0);
	~KosmosManagement();

private slots:
	void getMembers(QStringList headers, QList<QStringList> result);
	void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
	void showRetraitKosmos(const QString&);
	void showDepotKosmos(const QString&);
	void showDoEnchere(const QString&);
	void showKosmosHistoric(const QString&);
    void updatePhoto(QStringList infos, QString);
    void doubleClicked(const QModelIndex &current);

private:
	void displayMember(std::shared_ptr<Member> m, int abonnement);

	Ui::KosmosManagementClass ui;

    std::shared_ptr<Member> m_selectedMember;
    QHash<int,std::shared_ptr<Member>> m_member;
    std::unique_ptr<NetworkAdapter> m_networkAdapter;
	std::unique_ptr<NetworkAdapter> m_networkAdapterPhoto;
	std::unique_ptr<RetraitKosmos> m_retraitKosmos;
	std::unique_ptr<DepotKosmos> m_depotKosmos;
	std::unique_ptr<CreateEnchere> m_createEnchere;
	std::unique_ptr<KosmosHistoric> m_kosmosHistoric;
};

#endif // KOSMOSMANAGEMENT_H
