#ifndef FID_STATUT_MANAGER
#define FID_STATUT_MANAGER

#include "FidStatut.h"
#include "networkadapter.h"

#include <QObject>
#include <QMap>
#include <QStringList>

class FidStatutManager : public QObject
{
    Q_OBJECT

private:
    FidStatutManager(void);
    ~FidStatutManager(void);

signals:
    void isLoaded(void);

public:
    static FidStatutManager* getInstance();
    static void kill();
    bool add(std::shared_ptr<FidStatut> fidSatut, QWidget *caller);
    bool modify(FidStatut &fidSatutNew, std::shared_ptr<FidStatut> fidSatutOld, QWidget *caller);
    void remove(QString name, QWidget *caller);
    QStringList getHeaders();
    QMap<QString, std::shared_ptr<FidStatut>> getFidStatuts();
    std::shared_ptr<FidStatut> get(QString name);
    void setLastFidSatutAdded(std::shared_ptr<FidStatut> fidSatutAdded);

private slots:
    void getFidStatuts(QStringList headers, QList<QStringList> result);
    void fillId(int id);

private:
    static FidStatutManager *m_singleton;

    std::unique_ptr<NetworkAdapter> m_networkAdapter;
    QMap<QString, std::shared_ptr<FidStatut>> m_fidStatuts;
    QStringList m_headers;
    std::shared_ptr<FidStatut> m_fidStatutAdded;

};

#endif // FID_STATUT_MANAGER
