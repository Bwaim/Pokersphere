#ifndef CREATEKOSMOSOPERATION_H
#define CREATEKOSMOSOPERATION_H

#include <QDialog>
#include "ui_createkosmosoperation.h"
#include "Member.h"
#include "networkadapter.h"

class CreateKosmosOperation : public QDialog
{
	Q_OBJECT

public:
	CreateKosmosOperation(std::shared_ptr<Member> member, QWidget *parent = 0);
	~CreateKosmosOperation();

private slots:
	void cancelCreation(const QString&);
	void saveOperation(const QString&);

private:
	Ui::CreateKosmosOperationClass ui;

	std::shared_ptr<Member> m_member;
	std::unique_ptr<NetworkAdapter> m_networkAdapter;
};

#endif // CREATEKOSMOSOPERATION_H
