#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "ui_login.h"
#include "networkadapter.h"
#include "User.h"

class Login : public QDialog
{
	Q_OBJECT

public:
	Login(std::shared_ptr<User> user, QString title, QWidget *parent = 0);
	~Login();

private slots:
	void connexion();
	void quit();
	void verifyConnexion(QStringList headers, QList<QStringList> result);

private:
	Ui::LoginClass ui;

    std::shared_ptr<User> m_user;
    std::unique_ptr<NetworkAdapter> m_networkAdapter;
};

#endif // LOGIN_H
