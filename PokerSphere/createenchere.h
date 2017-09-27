#ifndef CREATEENCHERE_H
#define CREATEENCHERE_H

#include <QDialog>
#include "ui_createenchere.h"
#include "Member.h"
#include "networkadapter.h"

class CreateEnchere : public QDialog
{
	Q_OBJECT

public:
	CreateEnchere(std::shared_ptr<Member> member, QWidget *parent = 0);
	~CreateEnchere();

private slots:
	void setValue(int value);
	void cancelEnchere(const QString&);
	void saveEnchere(const QString&);
    void updatePhoto(QStringList infos, QString);

private:
	Ui::CreateEnchereClass ui;

	std::shared_ptr<Member> m_member;
	std::unique_ptr<NetworkAdapter> m_networkAdapter;
};

#endif // CREATEENCHERE_H
