#ifndef CHOOSESPONSOR_H
#define CHOOSESPONSOR_H

#include <memory>

#include <QDialog>
#include "ui_ChooseSponsor.h"
#include "Member.h"
#include "networkadapter.h"

class ChooseSponsor : public QDialog
{
    Q_OBJECT

public:
    ChooseSponsor(QWidget *parent = 0);
    ~ChooseSponsor();
    std::shared_ptr<Member> getSelection();

signals:
	void choose();

private slots:
	void getMembers(QStringList headers, QList<QStringList> result);
	void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
	void doFilter(bool);
	void choose(const QString&);
    void updatePhoto(QStringList infos, QString);
    void doubleClicked(const QModelIndex &current);

private:
    Ui::ChooseSponsorClass ui;

    std::unique_ptr<NetworkAdapter> m_networkAdapter;
	std::unique_ptr<NetworkAdapter> m_networkAdapterPhoto;
	QHash<int,std::shared_ptr<Member>> m_members;
	std::shared_ptr<Member> m_selectedMember;
};

#endif // CHOOSESPONSOR_H
