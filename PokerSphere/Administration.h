#ifndef ADMINISTRATION_H
#define ADMINISTRATION_H

#include <QWidget>
#include "ui_Administration.h"
#include "networkadapter.h"
#include "Statistiques.h"
#include "TournamentTemplateManagement.h"
#include "FidManagement.h"
#include "ClubManagement.h"
#include "DotationMenu.h"

class Administration : public QWidget
{
	Q_OBJECT

public:
	Administration(QWidget *parent = 0);
	~Administration();

private slots:
	void checkMonthlyCredit(const QDate&);
	void getCheckMonthCreditResult(QStringList headers, QList<QStringList> result);
	void doMonthlyCredit(const QString&);
	void showStatistiques(const QString&);
    void showTemplateTournamentManagement(const QString&);
    void showSponsorBonusManagement(const QString&);
    void feedFranchise(QStringList headers, QList<QStringList> result);
    void franchiseChange(int);
    void showQuery(const QString&);
    void showFidManagement(const QString&);
    void showClubManagement(const QString&);
    void showDotationMenu(const QString&);

private:
	Ui::AdministrationClass ui;

	std::unique_ptr<NetworkAdapter> m_networkAdapter;
    std::unique_ptr<NetworkAdapter> m_networkAdapterFranchise;
	bool m_monthOk;
	std::unique_ptr<Statistiques> m_statistiques;
    std::unique_ptr<TournamentTemplateManagement> m_tournamentTemplateManagement;
    std::unique_ptr<FidManagement> m_fidManagement;
    std::unique_ptr<ClubManagement> m_clubManagement;
    std::unique_ptr<DotationMenu> m_dotationMenu;
    int m_franchise;
};

#endif // ADMINISTRATION_H
