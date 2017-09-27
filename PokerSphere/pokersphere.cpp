#include "pokersphere.h"
#include "login.h"
#include "constantes.h"
#include "Parameter.h"
#include "requetes.h"

#include <QNetworkAccessManager>
#include <QProcess>
#include <QMessageBox>

static const char * VERSION  = "V1.6.1";

PokerSphere::PokerSphere(LogWindow &logWindow, QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags), m_membershipManagement(nullptr), m_createMembership(nullptr), m_administration(nullptr), m_kosmosManagement(nullptr)
    , m_tournamentManagement(nullptr), m_fidOperationsManagement(nullptr), m_logWindow(&logWindow)
{
	ui.setupUi(this);

#ifdef FREE
	QMessageBox::information(this,QString(INFOS),QString(MESSAGE_FREE),QMessageBox::Ok);
#endif

    if (!checkUpdate())
    {
        Parameter *param = Parameter::getInstance();

        QString title = QString("POKERSPHERE - ") + param->getTitle() + " - " + VERSION;
        setWindowTitle(title);

	    m_user = std::make_shared<User>();

	    Login login(m_user,title);
	    int rep = login.exec();

        m_logged = (rep == 0) ? false : true;

	    connect(ui.comptesAdherentsLabel,SIGNAL(linkActivated(QString)),this,SLOT(showMembershipManagement(QString)));
	    connect(ui.creerAdherentLabel,SIGNAL(linkActivated(QString)),this,SLOT(showCreateMembership(QString)));
	    connect(ui.gestionKosmosLabel,SIGNAL(linkActivated(QString)),this,SLOT(showKosmosManagement(QString)));
	    connect(ui.gestionTournoiLabel,SIGNAL(linkActivated(QString)),this,SLOT(showTournamentManagement(QString)));
        connect(ui.gestionSitnGoLabel,SIGNAL(linkActivated(QString)),this,SLOT(showSitnGoManagement(QString)));
	    connect(ui.administrationLabel,SIGNAL(linkActivated(QString)),this,SLOT(showAdministration(QString)));
        connect(ui.fidManagementLabel,SIGNAL(linkActivated(QString)),this,SLOT(showFidManagement(QString)));
    
        m_logWindow->showMinimized();
    }
}

PokerSphere::~PokerSphere()
{
    Parameter::kill();
}

void PokerSphere::closeEvent(QCloseEvent *event)
{
    //QMessageBox::information(this, "Fin de téléchargement", "PokerSphere::closeEvent");
    m_logWindow->setIsOkToClose(true);
    m_logWindow->close();
    event->accept();
    //QWidget::closeEvent(event);
}

bool PokerSphere::isLogged()
{
	return m_logged;
}

LogWindow* PokerSphere::getLogWindow()
{
    return m_logWindow;
}

void PokerSphere::showMembershipManagement(const QString&)
{
	m_membershipManagement.reset(new MembershipManagement());
	m_membershipManagement->show();
}

void PokerSphere::showCreateMembership(const QString&)
{
	m_createMembership.reset(new CreateMembership());
	m_createMembership->show();
}

void PokerSphere::showKosmosManagement(const QString&)
{
	m_kosmosManagement.reset(new KosmosManagement());
	m_kosmosManagement->show();
}

void PokerSphere::showTournamentManagement(const QString&)
{
	m_tournamentManagement.reset(new TournamentManagement(0, this));
	m_tournamentManagement->show();
}

void PokerSphere::showSitnGoManagement(const QString&)
{
    m_tournamentManagement.reset(new TournamentManagement(1, this));
    m_tournamentManagement->show();
}

void PokerSphere::showAdministration(const QString&)
{
	m_administration.reset(new Administration());
	m_administration->show();
}

void PokerSphere::showFidManagement(const QString&)
{
    m_fidOperationsManagement.reset(new FidOperationsManagement());
    m_fidOperationsManagement->show();
}

bool PokerSphere::checkUpdate()
{
#ifndef LOCAL
    QNetworkAccessManager manager;
    Parameter *param = Parameter::getInstance();
    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(param->getVersionFile()))); // Url vers le fichier version.txt
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    reply->deleteLater();
    QString versionNew = reply->readAll();

    QString updaterExe = "pks_updater";
#ifdef FREE
    updaterExe += "_Free";
#endif

    if (QString::compare(VERSION,versionNew) != 0)
    {
        //QProcess *qUpdater = new QProcess(this);
        QStringList args = QStringList(versionNew);
        //qUpdater->startDetached(updaterExe + ".exe ", args);
        QProcess::startDetached(updaterExe + ".exe ", args);
        QTimer::singleShot(100, this, SLOT(close()));
        return true;
    }
#endif
    return false;
}
