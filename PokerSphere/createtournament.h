#ifndef CREATETOURNAMENT_H
#define CREATETOURNAMENT_H

#include <QDialog>
#include "ui_createtournament.h"
#include "networkadapter.h"
#include "TournamentTemplateManager.h"

class CreateTournament : public QDialog
{
	Q_OBJECT

public:
	CreateTournament(int typeTournament, QWidget *parent = 0);
    CreateTournament(QString name, int buyIn, int nbPlayers, int variante, QString comment, bool privat, int typeTournament
        , QDateTime beginDate, QDateTime endDate, int lastAttachedTournament, QWidget *parent = 0);
	~CreateTournament();

private slots:
	void cancelCreation(const QString&);
	void saveTournament(const QString&);
    void updateTemplateList();
    void updateFromTemplate(const QString& templateName);
    void feedTournamentList(QStringList headers, QList<QStringList> result);

private:
    void init();
    Ui::CreateTournamentClass ui;

	std::unique_ptr<NetworkAdapter> m_networkAdapter;
    TournamentTemplateManager *m_tournamentTemplateManager;
    int m_typeTournament;
    int m_franchise;
    int m_lastAttachedTournament;
};

#endif // CREATETOURNAMENT_H
