#include "FidStatut.h"

FidStatut::FidStatut()
{

}

FidStatut::FidStatut(FidStatut const& fidStatut)
    :m_id(fidStatut.m_id), m_name(fidStatut.m_name), m_seuil(fidStatut.m_seuil)
{

}

FidStatut::FidStatut(QString name, int seuil)
    :m_name(name), m_seuil(seuil)
{

}

FidStatut::~FidStatut()
{

}

FidStatut& FidStatut::operator=(FidStatut const& fidStatut)
{
    if (this != &fidStatut)
    {
        m_id = fidStatut.m_id;
        m_name = fidStatut.m_name;
        m_seuil = fidStatut.m_seuil;
    }

    return *this;
}

int FidStatut::getId() const
{
    return m_id;
}

void  FidStatut::setId(int id)
{
    m_id = id;
}

QString FidStatut::getName() const
{
    return m_name;
}

void FidStatut::setName(QString name)
{
    m_name = name;
}

int FidStatut::getSeuil() const
{
    return m_seuil;
}

void FidStatut::setSeuil(int seuil)
{
    m_seuil = seuil;
}
