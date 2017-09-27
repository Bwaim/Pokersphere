#ifndef PARAMETER_H
#define PARAMETER_H

#include <QString>
#include <QMap>

class Parameter
{
private:
    Parameter(void);
    ~Parameter(void);

public:
    
	static Parameter* getInstance();
	static void kill();

    QString getUrl();
    QString getVersionFile();
    int getFranchise();
    QString getTitle();
    QString getFranchiseLabel(int id);

private:
	static Parameter *m_singleton;

    QString m_url;
    int m_franchise;
    QString m_title;
    QMap<int,QString> m_franchiseLabels;
};

#endif //PARAMETER_H