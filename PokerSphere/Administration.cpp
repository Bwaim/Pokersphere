#include "Administration.h"

#include <QDate>
#include <QMessageBox>
#include "requetes.h"
#include "SponsorBonusManagement.h"
#include "Parameter.h"
#include "FranchiseSelection.h"
#include "Query.h"

Administration::Administration(QWidget *parent)
    : QWidget(parent), m_networkAdapter(nullptr), m_monthOk(false), m_statistiques(nullptr)
    , m_tournamentTemplateManagement(nullptr), m_fidManagement(nullptr), m_clubManagement(nullptr)
    , m_dotationMenu(nullptr), m_franchise(0)
{
	ui.setupUi(this);

    Parameter *param = Parameter::getInstance();
    int franchise = param->getFranchise();

    /** Pokersphere FRANCE */
    if (franchise == 0)
    {
        m_networkAdapterFranchise.reset(new NetworkAdapter(SELECT_FRANCHISE,SELECT,QStringList(),this));
	    connect(m_networkAdapterFranchise.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(feedFranchise(QStringList, QList<QStringList>)));
        connect(ui.franchiseComboBox,SIGNAL(activated(int)),this,SLOT(franchiseChange(int)));
    }
    else
    {
        ui.franchiseComboBox->setHidden(true);
        m_franchise = franchise;
    }

	connect(ui.moisDateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(checkMonthlyCredit(QDate)));
	ui.moisDateEdit->setDate(QDate::currentDate());

	connect(ui.creditMensuelLabel,SIGNAL(linkActivated(QString)),this,SLOT(doMonthlyCredit(QString)));
	connect(ui.statistiquesLabel,SIGNAL(linkActivated(QString)),this,SLOT(showStatistiques(QString)));
    connect(ui.gestionTemplateLabel,SIGNAL(linkActivated(QString)),this,SLOT(showTemplateTournamentManagement(QString)));
    connect(ui.gestionBonusParrainLabel,SIGNAL(linkActivated(QString)),this,SLOT(showSponsorBonusManagement(QString)));
    connect(ui.gestionFidelisationLabel,SIGNAL(linkActivated(QString)),this,SLOT(showFidManagement(QString)));
    connect(ui.gestionClubLabel,SIGNAL(linkActivated(QString)),this,SLOT(showClubManagement(QString)));
    connect(ui.queryLabel,SIGNAL(linkActivated(QString)),this,SLOT(showQuery(QString)));
    connect(ui.gestionDotationLabel,SIGNAL(linkActivated(QString)),this,SLOT(showDotationMenu(QString)));

    ui.queryLabel->setHidden(true);
#ifndef FRANCE
    ui.gestionFidelisationLabel->setHidden(true);
    ui.gestionDotationLabel->setHidden(true);
#endif
}

Administration::~Administration()
{
}

void Administration::checkMonthlyCredit(const QDate&)
{
    if (m_franchise != 0)
    {
	    disconnect(ui.moisDateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(checkMonthlyCredit(QDate)));
	    QDate date = ui.moisDateEdit->date();
	    ui.moisDateEdit->setDate(QDate(date.year(), date.month(), 1));
	    connect(ui.moisDateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(checkMonthlyCredit(QDate)));

	    QStringList args = QStringList();
	    args.append(ui.moisDateEdit->date().toString(QString("dd/MM/yyyy")));
	    m_networkAdapter.reset(new NetworkAdapter(SELECT_CHECK_MONTH_CREDIT,SELECT,args,this));
	    connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getCheckMonthCreditResult(QStringList, QList<QStringList>)));
    }
    else
        QMessageBox::warning(this,QString("Attention"),QString("Vous devez choisir une franchise !"));
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void Administration::getCheckMonthCreditResult(QStringList headers, QList<QStringList> result)
{
	if (result.size() == 1 && result[0].size() == 1)
	{
		m_monthOk = result[0][0].compare(QString(OK)) == 0;
	}
}
#pragma GCC diagnostic pop

void Administration::doMonthlyCredit(const QString&)
{
	if (m_monthOk)
	{
		QStringList args = QStringList();
		args.append(ui.moisDateEdit->date().toString(QString("dd/MM/yyyy")));
        args.append(QString::number(m_franchise));

		m_networkAdapter.reset(new NetworkAdapter(CREATE_MONTHLY_CREDIT,INSERT,args,this));
		m_monthOk = false;
	}
	else
		QMessageBox::warning(this,CREDIT_MENSUEL,QString(MOIS_DEJA_CREDITE));
}

void Administration::showStatistiques(const QString&)
{
	m_statistiques.reset(new Statistiques(m_franchise));
	m_statistiques->show();
}

void Administration::showTemplateTournamentManagement(const QString&)
{
    m_tournamentTemplateManagement.reset(new TournamentTemplateManagement(m_franchise));
	m_tournamentTemplateManagement->show();
}

void Administration::showSponsorBonusManagement(const QString&)
{
    SponsorBonusManagement sponsorBonusManagement(this);
    sponsorBonusManagement.exec();
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void Administration::feedFranchise(QStringList headers, QList<QStringList> result)
{
    ui.franchiseComboBox->addItem(QString(""));
    for (int i =0; i < result.size(); i++)
        ui.franchiseComboBox->addItem(result[i][1]);
}
#pragma GCC diagnostic pop

void Administration::franchiseChange(int value)
{
    m_franchise = value;
    checkMonthlyCredit(ui.moisDateEdit->date());
}

void Administration::showQuery(const QString&)
{
    Query query(this);
    query.exec();
}

void Administration::showFidManagement(const QString&)
{
    m_fidManagement.reset(new FidManagement());
    m_fidManagement->show();
}

void Administration::showClubManagement(const QString&)
{
    if (m_franchise != 0)
    {
        m_clubManagement.reset(new ClubManagement(m_franchise));
        m_clubManagement->show();
    }
    else
        QMessageBox::warning(this,QString("Attention"),QString("Vous devez choisir une franchise !"));
}

void Administration::showDotationMenu(const QString&)
{
    m_dotationMenu.reset(new DotationMenu());
    m_dotationMenu->show();
}
