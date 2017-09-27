#include "FidType.h"

FidType::FidType()
{

}

FidType::FidType(FidType const& fidType)
    : m_id(fidType.m_id), m_name(fidType.m_name), m_ratio(fidType.m_ratio)
    , m_value(fidType.m_value), m_nbMonth(fidType.m_nbMonth)
{

}

FidType::~FidType(void)
{

}

FidType& FidType::operator=(FidType const& fidType)
{
    if (this != &fidType)
    {
        m_id = fidType.m_id;
        m_name= fidType.m_name;
        m_ratio = fidType.m_ratio;
        m_value = fidType.m_value;
        m_nbMonth = fidType.m_nbMonth;
    }

    return *this;
}

int FidType::getId() const
{
    return m_id;
}
void FidType::setId(int id)
{
    m_id = id;
}

QString FidType::getName() const
{
    return m_name;
}

void FidType::setName(QString name)
{
    m_name = name;
}

int FidType::getRatio() const
{
    return m_ratio;
}

void FidType::setRatio(int ratio)
{
    m_ratio = ratio;
}

int FidType::getValue() const
{
    return m_value;
}

void FidType::setValue(int value)
{
    m_value = value;
}

int FidType::getNbMonth() const
{
    return m_nbMonth;
}

void FidType::setNbMonth(int nbMonth)
{
    m_nbMonth = nbMonth;
}
