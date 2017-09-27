#include "login.h"
#include "constantes.h"
#include "requetes.h"
#include "NetworkAdapter.h"
#include "Transform.h"
#include <QStringList>

Login::Login(std::shared_ptr<User> user, QString title, QWidget *parent)
	: QDialog(parent),m_user(user), m_networkAdapter(nullptr)
{
	ui.setupUi(this);

    setWindowTitle(title);

	QObject::connect(ui.connexionButton,SIGNAL(clicked()),this, SLOT(connexion()));
	QObject::connect(ui.quitButton,SIGNAL(clicked()),this, SLOT(quit()));

#ifdef LOCAL
#ifdef BORDEAUX
    ui.userLineEdit->setText(QString("pksbdx"));
    ui.passLineEdit->setText(QString("sphere33"));
#else
#ifdef TOULOUSE
    ui.userLineEdit->setText(QString("pkstls"));
    ui.passLineEdit->setText(QString("sphere31"));
#else
    ui.userLineEdit->setText(QString("Fabien"));
    ui.passLineEdit->setText(QString("jjaf0603"));
#endif
#endif
#endif

//#ifdef FREE
//	QMessageBox::information(this,QString(INFOS),QString(MESSAGE_FREE),QMessageBox::Ok);
//#endif

}

Login::~Login()
{
}

void Login::connexion()
{
	ui.statusLabel->setText("");
	if (ui.userLineEdit->text().isEmpty())
	{
		ui.statusLabel->setText(MISSING_LOGIN);
		return;
	}

	if (ui.passLineEdit->text().isEmpty())
	{
		ui.statusLabel->setText(MISSING_PASSWORD);
		return;
	}

	QStringList arglist = QStringList();
	arglist.append(ui.userLineEdit->text());
	arglist.append(ui.passLineEdit->text());

	m_networkAdapter.reset(new NetworkAdapter(SELECT_LOGIN,SELECT,arglist,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(verifyConnexion(QStringList, QList<QStringList>)));
}

void Login::quit()
{
	this->reject();
}

void Login::verifyConnexion(QStringList headers, QList<QStringList> result)
{
	if (result.size() == 0)
		ui.statusLabel->setText(WRONG_LOGIN);
	else
	{
		*m_user = Transform::toUser(headers,result);
		this->accept();
	}
}
