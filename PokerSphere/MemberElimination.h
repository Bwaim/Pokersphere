#ifndef MEMBERELIMINATION_H
#define MEMBERELIMINATION_H

#include <QDialog>
#include <QMap>
#include "ui_MemberElimination.h"
#include "Member.h"
#include "Tournament.h"
#include "networkadapter.h"

class MemberElimination : public QDialog
{
	Q_OBJECT

public:
	MemberElimination(std::shared_ptr<Tournament> tournament, std::shared_ptr<Member> member, std::shared_ptr<QMap<int, bool>> positions, QWidget *parent = 0);
	~MemberElimination();

private slots:
	void cancelSortie(const QString&);
	void saveSortie(const QString&);
    void addDotation(const QString&);
    void deleteDotation(const QString&);

private:
	Ui::MemberEliminationClass ui;

    bool positionIsValide();

    std::shared_ptr<Tournament> m_tournament;
    std::shared_ptr<Member> m_member;
    std::shared_ptr<QMap<int, bool>> m_positions;
	std::unique_ptr<NetworkAdapter> m_networkAdapter;
    QList<QWidget*> m_dotationsWidgets;
};

#endif // MEMBERELIMINATION_H
