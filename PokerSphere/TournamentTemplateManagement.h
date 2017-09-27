#ifndef TOURNAMENTTEMPLATEMANAGEMENT_H
#define TOURNAMENTTEMPLATEMANAGEMENT_H

#include <QWidget>
#include "ui_TournamentTemplateManagement.h"
#include "CreateTournamentTemplate.h"
#include "TournamentTemplateManager.h"
#include "TournamentTemplate.h"

class TournamentTemplateManagement : public QWidget
{
    Q_OBJECT

public:
    TournamentTemplateManagement(int franchise, QWidget *parent = 0);
    ~TournamentTemplateManagement();

private slots:
    void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
	void showCreateTournamentTemplate(const QString&);
	void updateTournamentTemplate(const QString&);
    void dupliquerTournamentTemplate(const QString&);
	void deleteTournamentTemplate(const QString&);
    void getTournamentsTemplate();

private:
    
    void displayTournamentTemplate(std::shared_ptr<TournamentTemplate> tournamentTemplate);

    Ui::TournamentTemplateManagement ui;

    TournamentTemplateManager *m_tournamentTemplateManager;
    std::unique_ptr<CreateTournamentTemplate> m_createTournamentTemplate;
    std::shared_ptr<TournamentTemplate> m_selectedTournamentTemplate;
    int m_franchise;
};

#endif // TOURNAMENTTEMPLATEMANAGEMENT_H
