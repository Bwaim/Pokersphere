#ifndef FIDTYPE_H
#define FIDTYPE_H

#include <QString>

class FidType
{
public:
    FidType();
    FidType(FidType const& fidType);
    ~FidType(void);

    FidType& operator=(FidType const& fidType);

    int getId() const;
    void setId(int id);
    QString getName() const;
    void setName(QString name);
    int getRatio() const;
    void setRatio(int ratio);
    int getValue() const;
    void setValue(int value);
    int getNbMonth() const;
    void setNbMonth(int nbMonth);

private:
    int m_id;
    QString m_name;
    int m_ratio;
    int m_value;
    int m_nbMonth;
};

#endif // FIDTYPE_H
