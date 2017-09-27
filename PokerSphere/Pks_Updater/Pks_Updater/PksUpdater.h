#ifndef PKSUPDATER_H
#define PKSUPDATER_H

#include <QWidget>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "ui_PksUpdater.h"

class PksUpdater : public QWidget
{
    Q_OBJECT

public:
    PksUpdater(QString version, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~PksUpdater();

private slots:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void save();

private:
    Ui::PksUpdaterClass ui;

    QNetworkReply *m_reply;
    QNetworkAccessManager m_manager;
    QString m_filename;
};

#endif // PKSUPDATER_H
