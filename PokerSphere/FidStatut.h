#ifndef FID_STATUT
#define FID_STATUT

#include <QString>

class FidStatut
{
public:
    FidStatut();
    FidStatut(FidStatut const& fidStatut);
    FidStatut(QString name, int seuil);
    ~FidStatut();

    FidStatut& operator=(FidStatut const& fidStatut);

    int getId() const;
    void setId(int id);
    QString getName() const;
    void setName(QString name);
    int getSeuil() const;
    void setSeuil(int seuil);

private:
    int m_id;
    QString m_name;
    int m_seuil;
};

#endif // FID_STATUT
