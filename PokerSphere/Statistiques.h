#ifndef STATISTIQUES_H
#define STATISTIQUES_H

#include <QWidget>
#include <QFile>
#include "ui_Statistiques.h"
#include "networkadapter.h"

class Statistiques : public QWidget
{
	Q_OBJECT

public:
	Statistiques(int franchise, QWidget *parent = 0);
	~Statistiques();

private slots:
	void getStatistiques(QStringList headers, QList<QStringList> result);
	void exportMembers(QStringList headers, QList<QStringList> result);
	void exportMembers(const QString&);
	void exportMembersUpToDate(QStringList headers, QList<QStringList> result);
	void exportMembersUpToDate(const QString&);
	void exportMembersExpireNextWeek(QStringList headers, QList<QStringList> result);
	void exportMembersExpireNextWeek(const QString&);
	void exportMembersExpired(QStringList headers, QList<QStringList> result);
	void exportMembersExpired(const QString&);
	void exportAdhesion(QStringList headers, QList<QStringList> result);
	void exportAdhesion(const QString&);
    void exportUniquePlayers(const QDate& date);
    void exportUniquePlayers(QStringList headers, QList<QStringList> result);

private:
	Ui::StatistiquesClass ui;

	std::unique_ptr<NetworkAdapter> m_networkAdapter;
    int m_franchise;
};

#endif // STATISTIQUES_H
