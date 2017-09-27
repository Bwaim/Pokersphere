#include "sendRankingHero.h"

#include "requetes.h"

#include <QLabel>
#include <QMessageBox>

SendRankingHero::SendRankingHero(std::shared_ptr<Tournament> tournament, bool check, QWidget *parent)
    : QDialog(parent), m_tournament(tournament), m_networkAdapter(nullptr), m_args(QString()), m_playerArgs(QString()), m_finished(false), m_error(false)
{
    ui.setupUi(this);

#ifdef OVH 
    m_RankingHeroUrl = QUrl("http://webservices.rankinghero.com");
#else
    m_RankingHeroUrl = QUrl("http://webservices.dev.rankinghero.com/");
#endif

    connect(ui.annulerLabel,SIGNAL(linkActivated(QString)),this,SLOT(reject()));
    if (check)
    {
        ui.envoyerLabel->setText(QString("<a href=""a"" style=""text-decoration:none;""><font color=""#5E2F6A"">Valider</font></a>"));
        this->setWindowTitle("Validation des résultats");
        connect(ui.envoyerLabel,SIGNAL(linkActivated(QString)),this,SLOT(accept()));
    }
    else
        connect(ui.envoyerLabel,SIGNAL(linkActivated(QString)),this,SLOT(sendData(QString)));

    QStringList args = QStringList();
	args.append(QString::number(m_tournament->getId()));
	m_networkAdapter.reset(new NetworkAdapter(SELECT_TOURNAMENT_WINNER,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getWinners(QStringList, QList<QStringList>)));
}

SendRankingHero::~SendRankingHero()
{

}

void SendRankingHero::sendData(const QString&)
{
    addGeneralInfos();
    addTournamentInfos();
    m_args += m_playerArgs;
    m_args += "END=1";
    m_request = QNetworkRequest(m_RankingHeroUrl);
    m_networkAccessManager.reset(new QNetworkAccessManager());
    m_reply = m_networkAccessManager->post(m_request,QByteArray(m_args.toStdString().c_str()));

	connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(errorMessage(QNetworkReply::NetworkError)));
	connect(m_reply, SIGNAL(finished()), this, SLOT(end()));
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void SendRankingHero::getWinners(QStringList headers, QList<QStringList> result)
{
    for (int i = 0 ; i < result.size() ; i++)
	{
        QStringList list = result[i];

        QString label = list[0];
        QLabel *qlab = new QLabel(label);
        QFont font = QFont();
        font.setPointSize(15);
        font.setFamily("Arial Narrow");
        font.setBold(true);
        qlab->setFont(font);
        QPalette palette = qlab->palette();
        palette.setColor(QPalette::WindowText,QColor(255,255,255));
        qlab->setPalette(palette);
        ui.labelVerticalLayout->addWidget(qlab);

        m_playerArgs += "p[" + QString::number(i) + "][rank]=" + list[1] + "&";
        m_playerArgs += "p[" + QString::number(i) + "][firstname]=" + list[2] + "&";
        m_playerArgs += "p[" + QString::number(i) + "][lastname]=" + list[3] + "&";
        m_playerArgs += "p[" + QString::number(i) + "][earning]=" + list[4] + "&";
        m_playerArgs += "p[" + QString::number(i) + "][id]=" + list[5] + "&";
    }
}
#pragma GCC diagnostic pop

void SendRankingHero::addGeneralInfos()
{
    m_args += "api_key=bc1be28427c4233ff1b7981935a414b1&";
    m_args += "output=xml&";
    m_args += "service=add_tournament&";
}

void SendRankingHero::addTournamentInfos()
{
    m_args += "id=" + QString::number(m_tournament->getId()) + "&";
    m_args += "name=" + m_tournament->getName() + "&";
    m_args += "date_start=" + m_tournament->getBeginDate().toString("yyyy-MM-dd") + "&";
    m_args += "time_start=" + m_tournament->getBeginDate().toString("hh:mm") + "&";
    m_args += "buyin=" + QString::number(m_tournament->getBuyIn()) + "&";
}

void SendRankingHero::end()
{
	if (!m_finished && !m_error)
	{
		QString str = m_reply->readAll();
		m_reply->deleteLater();
		m_finished = true;
		bool res = formatResult(str);
        if (res)
		    accept();
        else
            reject();
	}
}

void SendRankingHero::errorMessage(QNetworkReply::NetworkError)
{
	if (!m_error)
	{
		m_error = true;
		QMessageBox::critical(this, ERREUR, ERREUR_MSG1 + m_reply->errorString() + "</em>");
		reject();
	}
}

bool SendRankingHero::formatResult(QString res)
{
    //QMessageBox::information(this, "Infos", res);
    QString success = "0";
    if (res.contains(QString("<success>")))
    {
        success = res.split("<success>")[1].split("</success>")[0];
    }

    return QString::compare(success,"1") == 0;
}
