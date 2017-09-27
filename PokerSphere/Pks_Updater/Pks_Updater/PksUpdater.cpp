#include "PksUpdater.h"

#include <QFile>
#include <QDir>
#include <QProcess>
#include <QMessageBox>

#ifdef LOCAL
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
PksUpdater::PksUpdater(QString version, QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
{
    ui.setupUi(this);

    m_filename = "setup_PokerSphere";

    QString env = "";

#ifdef BORDEAUX
    env = QString("_Bordeaux");
#else
#ifdef TOULOUSE
    env = QString("_Toulouse");
#else
#ifdef BIARRITZ
    env = QString("_Biarritz");
#else
#ifdef BISCARROSSE
    env = QString("_Biscarrosse");
#endif
#endif
#endif
#endif

#ifdef LOCAL 
    QString url = QString("http://localhost/Fabien/update/PokerSphere.exe");
    m_filename = "PokerSphere.exe";

#endif

    m_reply = m_manager.get(QNetworkRequest(QUrl(url)));
 
    connect(m_reply, SIGNAL(finished()), this, SLOT(save()));
    connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64, qint64)));
}
#ifdef LOCAL
#pragma GCC diagnostic pop
#endif

PksUpdater::~PksUpdater()
{

}

void PksUpdater::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal != -1)
    {
        ui.downloadProgressBar->setRange(0, bytesTotal);
        ui.downloadProgressBar->setValue(bytesReceived);
    }
}

void PksUpdater::save()
{
    m_reply->deleteLater();

    QString fullFilename = QDir::homePath() + "\\Downloads\\" + m_filename;
    
    QFile lastversion(fullFilename);
    
    if (lastversion.open(QIODevice::WriteOnly) )
    {
        lastversion.write(m_reply->readAll());
        lastversion.close();

        //QProcess *installeur = new QProcess(this);
        //installeur->start(fullFilename);
        QProcess::startDetached(fullFilename);
    }
    else
        QMessageBox::critical(this, "Update", "Erreur d'écriture du fichier " + fullFilename);

    close();
}
