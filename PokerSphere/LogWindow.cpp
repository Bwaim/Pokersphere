#include "LogWindow.h"
#include "requetes.h"
#include "AssignCardNumber.h"
#include "Parameter.h"
#include "FranchiseSelection.h"

#include <QMessageBox>

LogWindow::LogWindow(QWidget *parent)
    : QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint),
      m_networkAdapter(nullptr), m_isOkToClose(false)
{
    ui.setupUi(this);

    QStringList items = QStringList();
    items.append(QString());
    items.append(QString("Partie libre"));
    items.append(QString("Tournoi"));
    ui.typeLogComboBox->addItems(items);

    //ui.arrivePushButton->setDefault(true);

    connect(ui.arrivePushButton, SIGNAL(released()), this, SLOT(card_scanned()));

}

LogWindow::~LogWindow()
{
    emit closed();
}

void LogWindow::closeEvent(QCloseEvent *event)
{
    if (m_isOkToClose)
        QWidget::closeEvent(event);
    else
        event->ignore();
}

void LogWindow::setIsOkToClose(bool ok)
{
    m_isOkToClose = ok;
}

void LogWindow::card_scanned()
{
    if (ui.cardNumberLineEdit->text().compare(QString()) == 0)
        return;

    if (ui.typeLogComboBox->currentIndex() == 0)
    {
        QMessageBox::warning(this, "Infos", QString("Vous devez sélectionner un type de log avant de pouvoir scanner une carte"));
        return;
    }

    QString cardNumber = ui.cardNumberLineEdit->text();
    memberArrive(cardNumber);
}

void LogWindow::memberArrive(QString cardNumber)
{
    QStringList args = QStringList();
	args.append(QString(cardNumber));
    args.append(QString::number(ui.typeLogComboBox->currentIndex()));

    Parameter *param = Parameter::getInstance();
    int franchise = param->getFranchise();
    if (franchise == 0)
    {
        FranchiseSelection franchiseSelection(&franchise);
        franchiseSelection.exec();
    }
    args.append(QString::number(franchise));

	m_networkAdapter = std::unique_ptr<NetworkAdapter>(new NetworkAdapter(0,ARRIVE,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QString)), this ,SLOT(displayLog(QString)));
    connect(m_networkAdapter.get(), SIGNAL(finished_log_error(QString)), this ,SLOT(displayLogError(QString)));
}

void LogWindow::displayLog(QString message)
{
    ui.logTextBrowser->append(message);
    if (message.contains(QString("inscrit au tournoi")))
    {
        emit tournamentInscription();
    }
    ui.cardNumberLineEdit->setText(QString());
}

void LogWindow::displayLogError(QString message)
{
    if (QMessageBox::warning(this, "Infos", message, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        AssignCardNumber assignCardNumber(ui.cardNumberLineEdit->text(), this);
        assignCardNumber.exec();
    }
    ui.cardNumberLineEdit->setText(QString());
}
