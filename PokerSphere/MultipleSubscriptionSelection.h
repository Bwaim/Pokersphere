#ifndef MULTIPLESUBSCRIPTIONSELECTION_H
#define MULTIPLESUBSCRIPTIONSELECTION_H

#include <QDialog>
#include "ui_MultipleSubscriptionSelection.h"

#include "networkadapter.h"
#include "Member.h"
#include "Tournament.h"

class MultipleSubscriptionSelection : public QDialog
{
    Q_OBJECT

public:
    MultipleSubscriptionSelection(std::shared_ptr<Tournament> tournament, QString idMembers, QString email, QString name, QString firstname, QWidget *parent = 0);
    ~MultipleSubscriptionSelection();

private slots:
	void getMembers(QStringList headers, QList<QStringList> result);
	void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
	void create(const QString&);
	void subscribe(const QString&);
    void updatePhoto(QStringList infos, QString);
    void doubleClicked(const QModelIndex &current);

private:
    Ui::MultipleSubscriptionSelection ui;

    std::shared_ptr<Tournament> m_tournament;
    std::unique_ptr<NetworkAdapter> m_networkAdapter;
	std::unique_ptr<NetworkAdapter> m_networkAdapterPhoto;
    QHash<int,std::shared_ptr<Member>> m_members;
    std::shared_ptr<Member> m_selectedMember;
    QString m_email;
    QString m_name;
    QString m_firstname;
};

#endif // MULTIPLESUBSCRIPTIONSELECTION_H
