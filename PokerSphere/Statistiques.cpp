#include "Statistiques.h"

#include "requetes.h"
#include "Extraction.h"

#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QDate>

Statistiques::Statistiques(int franchise, QWidget *parent)
	: QWidget(parent), m_networkAdapter(nullptr), m_franchise(franchise)
{
	ui.setupUi(this);

    QStringList args = QStringList();
    args.append(QString::number(m_franchise));

	m_networkAdapter.reset(new NetworkAdapter(SELECT_STATISTIQUES,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getStatistiques(QStringList, QList<QStringList>)));
	connect(ui.nombreAdherentsLabel, SIGNAL(linkActivated(QString)), this, SLOT(exportMembers(QString)));
	connect(ui.nombreAdherentsAJourLabel, SIGNAL(linkActivated(QString)), this, SLOT(exportMembersUpToDate(QString)));
	connect(ui.nombreAdherentsQuiExpireLabel, SIGNAL(linkActivated(QString)), this, SLOT(exportMembersExpireNextWeek(QString)));
	connect(ui.nombreAdherentsExpiresLabel, SIGNAL(linkActivated(QString)), this, SLOT(exportMembersExpired(QString)));
	connect(ui.adhesionLabel, SIGNAL(linkActivated(QString)), this, SLOT(exportAdhesion(QString)));
    connect(ui.joueurUniqueBegindateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(exportUniquePlayers(QDate)));
    connect(ui.joueurUniqueEnddateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(exportUniquePlayers(QDate)));

}

Statistiques::~Statistiques()
{
}

void Statistiques::getStatistiques(QStringList headers, QList<QStringList> result)
{
	if (result.size() == 1)
	{
		for (int i = 0 ; i < result[0].size() ; i++)
		{
			if (headers[i] == NB_ADHERENT)
				ui.nbAdherentsSpinBox->setValue(result[0][i].toInt());
			else
				if (headers[i] == NB_ADHERENT_A_JOUR)
					ui.nbAdherentsAJourSpinBox->setValue(result[0][i].toInt());
				else
					if (headers[i] == NB_ADHERENT_QUI_EXPIRE)
						ui.nbAdherentsQuiExpireSpinBox->setValue(result[0][i].toInt());
					else
						if (headers[i] == NB_ADHERENT_EXPIRES)
							ui.nbAdherentsExpireSpinBox->setValue(result[0][i].toInt());
		}
	}
}

void Statistiques::exportMembers(const QString&)
{
    QStringList args = QStringList();
    args.append(QString::number(m_franchise));

	m_networkAdapter.reset(new NetworkAdapter(SELECT_STATISTIQUES_MEMBERS,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(exportMembers(QStringList, QList<QStringList>)));
}

void Statistiques::exportMembers(QStringList headers, QList<QStringList> result)
{
	QString filename = QFileDialog::getSaveFileName(this,QString(ENREGISTRER_FICHIER),QString("membres_") 
		+ QDate::currentDate().toString(QString("yyyyMMdd")) +QString(".csv"),"*.csv");

	if (!filename.isEmpty())
	{
        Extraction::extractToCSV(filename,headers,result);
	}
}

void Statistiques::exportMembersUpToDate(QStringList headers, QList<QStringList> result)
{
	QString filename = QFileDialog::getSaveFileName(this,QString(ENREGISTRER_FICHIER),QString("membres_a_jour_") 
		+ QDate::currentDate().toString(QString("yyyyMMdd")) +QString(".csv"),"*.csv");

	if (!filename.isEmpty())
	{
        Extraction::extractToCSV(filename,headers,result);
	}
}

void Statistiques::exportMembersUpToDate(const QString&)
{
    QStringList args = QStringList();
    args.append(QString::number(m_franchise));

	m_networkAdapter.reset(new NetworkAdapter(SELECT_STATISTIQUES_MEMBERS_UPTODATE,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(exportMembersUpToDate(QStringList, QList<QStringList>)));
}

void Statistiques::exportMembersExpireNextWeek(QStringList headers, QList<QStringList> result)
{
	QString filename = QFileDialog::getSaveFileName(this,QString(ENREGISTRER_FICHIER),QString("membres_qui_expirent_") 
		+ QDate::currentDate().toString(QString("yyyyMMdd")) +QString(".csv"),"*.csv");

	if (!filename.isEmpty())
	{
        Extraction::extractToCSV(filename,headers,result);
	}
}

void Statistiques::exportMembersExpireNextWeek(const QString&)
{
    QStringList args = QStringList();
    args.append(QString::number(m_franchise));

	m_networkAdapter.reset(new NetworkAdapter(SELECT_STATISTIQUES_MEMBERS_EXPIRE_NEXT_WEEK,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(exportMembersExpireNextWeek(QStringList, QList<QStringList>)));
}

void Statistiques::exportMembersExpired(QStringList headers, QList<QStringList> result)
{
	QString filename = QFileDialog::getSaveFileName(this,QString(ENREGISTRER_FICHIER),QString("membres_expires_") 
		+ QDate::currentDate().toString(QString("yyyyMMdd")) +QString(".csv"),"*.csv");

	if (!filename.isEmpty())
	{
        Extraction::extractToCSV(filename,headers,result);
	}
}

void Statistiques::exportMembersExpired(const QString&)
{
    QStringList args = QStringList();
    args.append(QString::number(m_franchise));

	m_networkAdapter.reset(new NetworkAdapter(SELECT_STATISTIQUES_MEMBERS_EXPIRED,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(exportMembersExpired(QStringList, QList<QStringList>)));
}

void Statistiques::exportAdhesion(QStringList headers, QList<QStringList> result)
{
	QString filename = QFileDialog::getSaveFileName(this,QString(ENREGISTRER_FICHIER),QString("adhesion_") 
		+ ui.adhesionBeginDateEdit->date().toString(QString("yyyyMM")) + QString("_") 
		+ ui.adhesionEndDateEdit->date().toString(QString("yyyyMM")) + QString(".csv"),"*.csv");

	if (!filename.isEmpty())
	{
        Extraction::extractToCSV(filename,headers,result);
	}
}

void Statistiques::exportAdhesion(const QString&)
{
	QStringList args = QStringList();
	args.append(ui.adhesionBeginDateEdit->date().toString(QString("yyyyMM")));
	args.append(ui.adhesionEndDateEdit->date().toString(QString("yyyyMM")));
    args.append(QString::number(m_franchise));

	m_networkAdapter.reset(new NetworkAdapter(SELECT_STATISTIQUES_ADHESION,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(exportAdhesion(QStringList, QList<QStringList>)));
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void Statistiques::exportUniquePlayers(const QDate& date)
{
    QStringList args = QStringList();
	args.append(ui.joueurUniqueBegindateEdit->date().toString(QString("yyyyMM")));
	args.append(ui.joueurUniqueEnddateEdit->date().toString(QString("yyyyMM")));
    args.append(QString::number(m_franchise));

	m_networkAdapter.reset(new NetworkAdapter(SELECT_STATISTIQUES_UNIQUE_PLAYER,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(exportUniquePlayers(QStringList, QList<QStringList>)));
}
#pragma GCC diagnostic pop

#pragma GCC diagnostic ignored "-Wunused-parameter"
void Statistiques::exportUniquePlayers(QStringList headers, QList<QStringList> result)
{
    ui.nbJoueurUniqueSpinBox->setValue(result[0][0].toInt());
}
#pragma GCC diagnostic pop
