#ifndef CREATETOURNAMENTTEMPLATE_H
#define CREATETOURNAMENTTEMPLATE_H

#include <QDialog>
#include <QString>
#include "ui_CreateTournamentTemplate.h"
#include "TournamentTemplateManager.h"

class CreateTournamentTemplate : public QDialog
{
    Q_OBJECT

public:
    CreateTournamentTemplate(int franchise, QWidget *parent = 0);
    CreateTournamentTemplate(QString name, int buyIn, int nbPlayers, int variante, QString comment, int franchise, QWidget *parent = 0);
    ~CreateTournamentTemplate();

private slots:
	void cancelCreation(const QString&);
	void saveTournamentTemplate(const QString&);

private:
    void init(int franchise);

    Ui::CreateTournamentTemplate ui;

    TournamentTemplateManager *m_tournamentTemplateManager;
};

#endif // CREATETOURNAMENTTEMPLATE_H
