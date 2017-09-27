#ifndef ASSIGNCARDNUMBER_H
#define ASSIGNCARDNUMBER_H

#include <QDialog>
#include <QString>
#include "Member.h"
#include "networkadapter.h"
#include "ui_AssignCardNumber.h"

class AssignCardNumber : public QDialog
{
    Q_OBJECT

public:
    AssignCardNumber(QString cardNumber, QWidget *parent = 0);
    ~AssignCardNumber();

private slots:
	void getMembers(QStringList headers, QList<QStringList> result);
	void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
	void doFilter(bool);
	void assign(const QString&);
    void updatePhoto(QStringList infos, QString);
    void doubleClicked(const QModelIndex &current);

private:
    Ui::AssignCardNumberClass ui;

    QString m_cardNumber;
    std::unique_ptr<NetworkAdapter> m_networkAdapter;
	std::unique_ptr<NetworkAdapter> m_networkAdapterPhoto;
	QHash<int,std::shared_ptr<Member>> m_members;
	std::shared_ptr<Member> m_selectedMember;
};

#endif // ASSIGNCARDNUMBER_H
