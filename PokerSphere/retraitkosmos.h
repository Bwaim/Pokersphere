#ifndef RETRAITKOSMOS_H
#define RETRAITKOSMOS_H

#include <QDialog>
#include "ui_retraitkosmos.h"
#include "Member.h"
#include "networkadapter.h"

class RetraitKosmos : public QDialog
{
	Q_OBJECT

public:
	RetraitKosmos(std::shared_ptr<Member> member, QWidget *parent = 0);
	~RetraitKosmos();

private slots:
	void setValue(int value);
	void cancelRetrait(const QString&);
	void saveRetrait(const QString&);
    void updatePhoto(QStringList infos, QString);

private:
	Ui::RetraitKosmosClass ui;

	std::shared_ptr<Member> m_member;
	std::unique_ptr<NetworkAdapter> m_networkAdapter;
};

#endif // RETRAITKOSMOS_H
