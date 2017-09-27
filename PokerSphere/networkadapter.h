#ifndef NETWORKADAPTER_H
#define NETWORKADAPTER_H

#include <memory>

#include <QProgressBar>
#include "ui_networkadapter.h"
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QString>
#include <QByteArray>

class NetworkAdapter : public QProgressBar
{
	Q_OBJECT

public:
	NetworkAdapter(int req, QString type, QStringList arg, QWidget *parent = 0);
	~NetworkAdapter();

    static QString formatArguments(QStringList args);

signals:
	void finished(QStringList,QList<QStringList>);
	void finished();
	void finished(QString);
    void finished(int);
    void finished(QStringList,QString);
    void finished_log_error(QString);

private slots:
	void end();
	void errorMessage(QNetworkReply::NetworkError);
	void requestInProgress(qint64, qint64);

private:
	void formatResult(QString res);

	Ui::NetworkAdapterClass ui;
	bool m_finished;
	bool m_error;
	QUrl m_url;
	QNetworkRequest m_request;
	std::unique_ptr<QNetworkAccessManager> m_networkAccessManager;
	QNetworkReply *m_reply;
};

#endif // NETWORKADAPTER_H
