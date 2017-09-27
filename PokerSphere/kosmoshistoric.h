#ifndef KOSMOSHISTORIC_H
#define KOSMOSHISTORIC_H

#include <QDialog>
#include "ui_kosmoshistoric.h"
#include "Member.h"
#include "networkadapter.h"
#include "Kosmos.h"
#include "createkosmosoperation.h"
#include "KosmosExtraction.h"

class KosmosHistoric : public QDialog
{
	Q_OBJECT

public:
	KosmosHistoric(std::shared_ptr<Member> member,QWidget *parent = 0);
	~KosmosHistoric();

private slots:
	void getKosmos(QStringList headers, QList<QStringList> result);
	void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
	void showCreateKosmos(const QString&);
	void updateKosmos(const QString&);
	void deleteKosmos(const QString&);
    void extractKosmos(const QString&);

private:
	void displayKosmos(std::shared_ptr<Kosmos> k);

	Ui::KosmosHistoricClass ui;

	std::shared_ptr<Member> m_member;
    std::shared_ptr<Kosmos> m_selectedKosmos;
    std::unique_ptr<NetworkAdapter> m_networkAdapter;
	QHash<int,std::shared_ptr<Kosmos>> m_kosmos;
	std::unique_ptr<CreateKosmosOperation> m_createKosmosOperation;
    std::unique_ptr<KosmosExtraction> m_kosmosExtraction;
};

#endif // KOSMOSHISTORIC_H
