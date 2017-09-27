#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QDialog>
#include <QCloseEvent>
#include "ui_LogWindow.h"
#include "networkadapter.h"

class LogWindow : public QDialog
{
    Q_OBJECT

public:
    LogWindow(QWidget *parent = 0);
    ~LogWindow();

    void setIsOkToClose(bool ok);

signals:
    void closed();
    void tournamentInscription();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void card_scanned();
    void displayLog(QString message);
    void displayLogError(QString message);

private:
    Ui::LogWindow ui;

    std::unique_ptr<NetworkAdapter> m_networkAdapter;
    bool m_isOkToClose;

    void memberArrive(QString cardNumber);
};

#endif // LOGWINDOW_H
