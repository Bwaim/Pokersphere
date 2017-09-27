#include "MembershipManager.h"

//Initialisation du singleton à NULL
MembershipManager *MembershipManager::m_singleton = nullptr;
const char *MembershipManager::ABONNEMENT[MembershipManager::TAILLE_ABONNEMENT] = {"Soirée découverte", "Adhésion 1 mois", "Adhésion 3 mois"
		, "Adhésion 6 mois", "Adhésion 12 mois", "Conversion"
		, "Conversion refusée", "Adhésion Online", "Pass One Shot"};
const int MembershipManager::END_DATE_ECART[MembershipManager::TAILLE_ABONNEMENT] = {1, 1, 3, 6, 12, 0, 0, 12, 1 };
const bool MembershipManager::IS_MONTH[MembershipManager::TAILLE_ABONNEMENT] = { false, true, true, true, true, true, true, true, false };

MembershipManager::MembershipManager()
{
	m_displayList = QStringList();
	m_endDateValue = QHash<QString,int>();
	m_isMonth = QHash<QString,bool>();
	for ( int i = 0 ; i < TAILLE_ABONNEMENT ; i++)
	{
		if (i != 5 && i != 6)
			m_displayList.push_back(QString(ABONNEMENT[i]));
		m_value.insert(QString(ABONNEMENT[i]), i);
		m_endDateValue.insert(QString(ABONNEMENT[i]), END_DATE_ECART[i]);
		m_isMonth.insert(QString(ABONNEMENT[i]), IS_MONTH[i]);
	}
}


MembershipManager::~MembershipManager()
{
}

MembershipManager* MembershipManager::getInstance()
{
    if (!m_singleton)
	{
		m_singleton = new MembershipManager();
	}

	return m_singleton;
}

void MembershipManager::kill()
{
	if (m_singleton)
	{
		delete m_singleton;
		m_singleton = nullptr;
	}
}

QStringList MembershipManager::getDisplayList()
{
	return m_displayList;
}

QDate MembershipManager::getEndDate(QString membership, QDate beginDate)
{
	if (m_isMonth.value(membership))
		return beginDate.addMonths(m_endDateValue.value(membership));
	else
		return beginDate.addDays(m_endDateValue.value(membership));
}

int MembershipManager::getValue(QString membership)
{
	return m_value.value(membership);
}

int MembershipManager::getDisplayIndex(int globalIndex)
{
	return m_displayList.indexOf(QString(ABONNEMENT[globalIndex]));
}

QString MembershipManager::getLabel(int index)
{
	return QString(ABONNEMENT[index]);
}

int MembershipManager::getNbMonth(QString membership)
{
    if (m_isMonth.value(membership))
        return m_endDateValue.value(membership);
    else
        return 0;
}
