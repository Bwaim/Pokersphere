#include "SponsorBonus.h"


SponsorBonus::SponsorBonus(void)
{
}

SponsorBonus::SponsorBonus(SponsorBonus const& bonus)
    : m_id(bonus.m_id), m_launching(bonus.m_launching), m_typeGodsonGain(bonus.m_typeGodsonGain), m_godsonGain(bonus.m_godsonGain),
    m_typeSponsorGain(bonus.m_typeSponsorGain), m_sponsorGain(bonus.m_sponsorGain), m_nbMonthFirstMembership(bonus.m_nbMonthFirstMembership)
{

}

SponsorBonus::~SponsorBonus(void)
{
}

SponsorBonus& SponsorBonus::operator=(SponsorBonus const& bonus)
{
    if (this != &bonus)
	{
		m_id = bonus.m_id;
		m_launching = bonus.m_launching;
        m_typeGodsonGain = bonus.m_typeGodsonGain;
        m_godsonGain = bonus.m_godsonGain;
        m_typeSponsorGain = bonus.m_typeSponsorGain;
        m_sponsorGain = bonus.m_sponsorGain;
        m_nbMonthFirstMembership = bonus.m_nbMonthFirstMembership;
	}

	return *this;
}

int SponsorBonus::getId() const
{
	return m_id;
}

void SponsorBonus::setId(int id)
{
	m_id = id;
}

int SponsorBonus::getLaunching() const
{
    return m_launching;
}

void SponsorBonus::setLaunching(int launching)
{
    m_launching = launching;
}

int SponsorBonus::getTypeGodsonGain() const
{
    return m_typeGodsonGain;
}

void SponsorBonus::setTypeGodsonGain(int typeGodsonGain)
{
    m_typeGodsonGain = typeGodsonGain;
}

int SponsorBonus::getGodsonGain() const
{
    return m_godsonGain;
}

void SponsorBonus::setGodsonGain(int godsonGain)
{
    m_godsonGain = godsonGain;
}

int SponsorBonus::getTypeSponsorGain() const
{
    return m_typeSponsorGain;
}

void SponsorBonus::setTypeSponsorGain(int typeSponsorGain)
{
    m_typeSponsorGain = typeSponsorGain;
}

int SponsorBonus::getSponsorGain() const
{
    return m_sponsorGain;
}

void SponsorBonus::setSponsorGain(int sponsorGain)
{
    m_sponsorGain = sponsorGain;
}

int SponsorBonus::getNbMonthFirstMembership() const
{
    return m_nbMonthFirstMembership;
}

void SponsorBonus::setNbMonthFirstMembership(int nbMonthFirstMembership)
{
    m_nbMonthFirstMembership = nbMonthFirstMembership;
}
