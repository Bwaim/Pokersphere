#ifndef MEMBERSHIP_MANAGER_H
#define MEMBERSHIP_MANAGER_H

#include <QStringList>
#include <QHash>
#include <QDate>
#include <QString>

class MembershipManager
{
private:
	MembershipManager();
    ~MembershipManager();

public:
	static MembershipManager* getInstance();
	static void kill();

	QStringList getDisplayList();
	QDate getEndDate(QString membership, QDate beginDate);
	int getValue(QString membership);
	int getDisplayIndex(int globalIndex);
	QString getLabel(int index);
    int getNbMonth(QString membership);

private:
	static MembershipManager *m_singleton;

	static const int TAILLE_ABONNEMENT = 9;
	static const char *ABONNEMENT[TAILLE_ABONNEMENT];
	static const int END_DATE_ECART[TAILLE_ABONNEMENT];
	static const bool IS_MONTH[TAILLE_ABONNEMENT];

	QStringList m_displayList;
	QHash<QString, int> m_value;
	QHash<QString, int> m_endDateValue;
	QHash<QString, bool> m_isMonth;
};

#endif //MEMBERSHIP_MANAGER_H
