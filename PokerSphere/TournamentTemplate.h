#ifndef TOURNAMENT_TEMPLATE
#define TOURNAMENT_TEMPLATE

#include <QString>

class TournamentTemplate
{
public:
    TournamentTemplate(void);
    TournamentTemplate(TournamentTemplate const& tournamentTemplate);
    TournamentTemplate(QString name, int buyIn, int nbPlayers, int variante, QString comment);
    ~TournamentTemplate(void);

    TournamentTemplate& operator=(TournamentTemplate const& tournamentTemplate);

    int getId() const;
	void setId(int id);
	QString getName() const;
	void setName(QString name);
    int getBuyIn() const;
	void setBuyIn(int buyIn);
	int getNbPlayers() const;
	void setNbPlayers(int nbPlayers);
	int getVariante() const;
	void setVariante(int variante);
    bool getPrivate() const;
    void setPrivate(bool privat);
    QString getComment() const;
	void setComment(QString comment);
    void setFranchise(int franchise);
	int getFranchise() const;

protected:
    int m_id;
	QString m_name;
    int m_buyIn;
	int m_nbPlayers;
	int m_variante;
    bool m_private;
    QString m_comment;
    int m_franchise;
    
};

#endif // TOURNAMENT_TEMPLATE