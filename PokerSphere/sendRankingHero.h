#ifndef SENDRANKINGHERO_H
#define SENDRANKINGHERO_H

#include <QDialog>
#include "ui_sendRankingHero.h"
#include "Tournament.h"
#include "networkadapter.h"

class SendRankingHero : public QDialog
{
    Q_OBJECT

public:
    SendRankingHero(std::shared_ptr<Tournament> tournament, bool check = false, QWidget *parent = 0);
    ~SendRankingHero();

private slots:
    void getWinners(QStringList headers, QList<QStringList> result);
    void sendData(const QString&);
    void end();
	void errorMessage(QNetworkReply::NetworkError);

private:
    Ui::SendRankingHeroClass ui;

    void addGeneralInfos();
    void addTournamentInfos();
    bool formatResult(QString res);

    std::shared_ptr<Tournament> m_tournament;
    std::unique_ptr<NetworkAdapter> m_networkAdapter;
    QUrl m_RankingHeroUrl;
    QNetworkRequest m_request;
    std::unique_ptr<QNetworkAccessManager> m_networkAccessManager;
    QNetworkReply *m_reply;
    QString m_args;
    QString m_playerArgs;
    bool m_finished;
	bool m_error;
};

#endif // SENDRANKINGHERO_H
