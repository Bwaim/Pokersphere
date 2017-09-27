#ifndef TOURNAMENT_TEMPLATE_MANAGER
#define TOURNAMENT_TEMPLATE_MANAGER

#include "TournamentTemplate.h"
#include "networkadapter.h"

#include <QMap>
#include <QObject>
#include <QStringList>

class TournamentTemplateManager : public QObject
{
    Q_OBJECT

private:
    TournamentTemplateManager(void);
    ~TournamentTemplateManager(void);
    
signals:
    void isLoaded(void);

public:
    
	static TournamentTemplateManager* getInstance(int franchise);
	static void kill();
    bool add(std::shared_ptr<TournamentTemplate> tournamentTemplate, QWidget *caller);
    bool modify(TournamentTemplate &tournamentTemplateNew, std::shared_ptr<TournamentTemplate> tournamentTemplateOld, QWidget *caller);
    void remove(QString name, QWidget *caller);
    QStringList getHeaders();
    QMap<QString, std::shared_ptr<TournamentTemplate>> getTemplates();
    std::shared_ptr<TournamentTemplate> get(QString name);
    void setLastTemplateAdded(std::shared_ptr<TournamentTemplate> templateAdded);

private slots:
	void getTournamentsTemplate(QStringList headers, QList<QStringList> result);
    void fillId(int id);

private:
	static TournamentTemplateManager *m_singleton;

    std::unique_ptr<NetworkAdapter> m_networkAdapter;
    QMap<QString, std::shared_ptr<TournamentTemplate>> m_tournamentsTemplate;
    QStringList m_headers;
    std::shared_ptr<TournamentTemplate> m_templateAdded;
    static int m_franchise;
};

#endif //TOURNAMENT_TEMPLATE_MANAGER