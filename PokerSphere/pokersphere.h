#ifndef POKERSPHERE_H
#define POKERSPHERE_H

#include <QMainWindow>
#include <QCloseEvent>
#include "ui_pokersphere.h"
#include "User.h"
#include "MembershipManagement.h"
#include "createmembership.h"
#include "kosmosmanagement.h"
#include "tournamentmanagement.h"
#include "Administration.h"
#include "LogWindow.h"
#include "FidOperationsManagement.h"



class PokerSphere : public QMainWindow
{
	Q_OBJECT

public:
    PokerSphere(LogWindow &logWindow, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~PokerSphere();

	bool isLogged();
    LogWindow* getLogWindow();

public slots:
	void showMembershipManagement(const QString&);
	void showCreateMembership(const QString&);
	void showKosmosManagement(const QString&);
	void showTournamentManagement(const QString&);
    void showSitnGoManagement(const QString&);
	void showAdministration(const QString&);
    void showFidManagement(const QString&);

protected:
    void closeEvent(QCloseEvent *event);

private:
    bool checkUpdate();

	Ui::PokerSphereClass ui;
	bool m_logged;
	std::shared_ptr<User> m_user;
	std::unique_ptr<MembershipManagement> m_membershipManagement;
	std::unique_ptr<CreateMembership> m_createMembership;
    std::unique_ptr<Administration> m_administration;
    std::unique_ptr<KosmosManagement> m_kosmosManagement;
	std::unique_ptr<TournamentManagement> m_tournamentManagement;
    std::unique_ptr<FidOperationsManagement> m_fidOperationsManagement;
    LogWindow *m_logWindow;

};

#endif // POKERSPHERE_H
