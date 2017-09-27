#ifndef DEPOTKOSMOS_H
#define DEPOTKOSMOS_H

#include <QDialog>
#include "ui_depotkosmos.h"
#include "Member.h"
#include "networkadapter.h"

class DepotKosmos : public QDialog
{
	Q_OBJECT

public:
	DepotKosmos(std::shared_ptr<Member> member, QWidget *parent = 0);
	~DepotKosmos();

private slots:
	void setValue(int value);
	void cancelDepot(const QString&);
	void saveDepot(const QString&);
    void updatePhoto(QStringList infos, QString);

private:
	Ui::DepotKosmosClass ui;

	std::shared_ptr<Member> m_member;
	std::unique_ptr<NetworkAdapter> m_networkAdapter;
};

#endif // DEPOTKOSMOS_H
