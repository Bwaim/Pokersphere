#include "networkadapter.h"
#include <QMessageBox>
#include <QList>
#include "requetes.h"
#include "Parameter.h"

NetworkAdapter::NetworkAdapter(int req, QString type, QStringList arg, QWidget *parent)
	: QProgressBar(parent)
{
	ui.setupUi(this);

    Parameter *param = Parameter::getInstance();

	m_finished = false;
	m_error = false;
	m_url = QUrl(param->getUrl());
	m_request = QNetworkRequest(m_url);
	m_networkAccessManager.reset(new QNetworkAccessManager());

	QString args = QString("requete=") + QString::number(req) + QString("&type=") + type + QString("&") + QString("&franchise=") +  QString::number(param->getFranchise()) + QString("&");
	args += formatArguments(arg);

    m_request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
    //m_request.setRawHeader( "charset", "iso-8859-15" );

	m_reply = m_networkAccessManager->post(m_request,QByteArray(args.toStdString().c_str()));

	connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(errorMessage(QNetworkReply::NetworkError)));
	connect(m_reply, SIGNAL(finished()), this, SLOT(end()));
	connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(requestInProgress(qint64, qint64) ));

	//show();
	hide();
}

NetworkAdapter::~NetworkAdapter()
{
}

void NetworkAdapter::end()
{
	if (!m_finished && !m_error)
	{
        QString str = m_reply->readAll();
        //QString str = QString::fromUtf8(m_reply->readAll());
        //QString str = QString::fromLatin1(m_reply->readAll());
		m_reply->deleteLater();
		m_finished = true;
		formatResult(str);
		close();
	}
}

void NetworkAdapter::errorMessage(QNetworkReply::NetworkError)
{
	if (!m_error)
	{
		m_error = true;
		QMessageBox::critical(this, ERREUR, ERREUR_MSG1 + m_reply->errorString() + "</em>");
		close();
	}
}

void NetworkAdapter::requestInProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	if (bytesTotal != -1)
    {
        this->setRange(0, bytesTotal);
        this->setValue(bytesReceived);
		//show();
    }
}

QString NetworkAdapter::formatArguments(QStringList args)
{
	QString str = QString("nbArg=") + QString::number(args.size());

	for ( int i = 0 ; i < args.size() ; i++)
	{
		str += QString("&arg") + QString::number(i) + QString("=") + args[i];
	}

	return str;
}

void NetworkAdapter::formatResult(QString res)
{
	if (res.contains(QString("ERREUR")))
	{
        if (res.contains(QString("<CONTINUE>")))
        {
            QMessageBox::critical(this, "Erreur", res.split("<ERREUR>")[1].split("<CONTINUE>")[1]);
            emit finished();
		    close();
		    return;
        }
		QMessageBox::critical(this, "Erreur", res.split("<ERREUR>")[1]);
		close();
		return;
	}

    if (res.contains(QString("<ID>")))
	{
		emit finished(res.split("<ID>")[1].toInt());
	}

	if (res.contains(QString("MESSAGE")))
	{
		QMessageBox::information(this, "Infos", res.split("<MESSAGE>")[1]);
		emit finished();
		close();
		return;
	}

	if (res.contains(QString("<GET_PHOTO>")))
	{
        QStringList tmplist = res.split("<GET_PHOTO>");
        QStringList infos = tmplist[0].split(SEPARATOR);
        emit finished(infos,tmplist[1]);
		close();
		return;
	}

	if (res.contains(QString("<PUT_PHOTO>")))
	{
		emit finished();
		close();
		return;
	}

    if (res.contains(QString("<LOG_ERROR>")))
	{
		emit finished_log_error(res.split("<LOG_ERROR>")[1]);
		close();
		return;
	}

    if (res.contains(QString("<LOG>")))
	{
        QStringList listMsg = res.split("<LOG>")[1].split("<LOG_ALERT>");
        QStringList msg = listMsg[0].split("<LOG_SEP>",QString::SkipEmptyParts);
        for (int i=0; i < msg.size(); i++)
		{
			emit finished(msg[i]);
		}

        if (listMsg.size() > 1)
        {
            QString alert = listMsg[1];
            if (!alert.isEmpty())
            {
                QMessageBox::information(this, "Infos", alert);
            }
        }
		
		close();
		return;
	}

	QStringList tmplist = res.split(HEADER_SEP,QString::SkipEmptyParts);
	QStringList headers = QStringList();
	QList<QStringList> result = QList<QStringList>();

	if (tmplist.size() > 0)
	{
		headers = tmplist[0].split(SEPARATOR);
	}

	if (tmplist.size() > 1)
	{
		QStringList lines = tmplist[1].split(LINE_SEP,QString::SkipEmptyParts);
		for (int i=0; i < lines.size(); i++)
		{
			result += lines[i].split(SEPARATOR);
		}
	}

	emit finished(headers,result);
	close();
}
