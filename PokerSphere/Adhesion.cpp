#include "Adhesion.h"


Adhesion::Adhesion(void)
	: m_member(nullptr)
{
}

Adhesion::Adhesion(Adhesion const& adhesion)
	: m_id(adhesion.m_id), m_value(adhesion.m_value), m_operationDate(adhesion.m_operationDate), m_beginDate(adhesion.m_beginDate)
	, m_endDate(adhesion.m_endDate), m_prix(adhesion.m_prix)
{
	m_member = std::make_shared<Member>(*adhesion.m_member);
}

Adhesion::~Adhesion(void)
{
}

Adhesion& Adhesion::operator=(Adhesion const& adhesion)
{
	if (this != &adhesion)
	{
		m_id = adhesion.m_id;
		m_member = std::make_shared<Member>(*adhesion.m_member);
		m_value = adhesion.m_value;
		m_operationDate = adhesion.m_operationDate;
		m_beginDate = adhesion.m_beginDate;
		m_endDate = adhesion.m_endDate;
        m_prix = adhesion.m_prix;
	}

	return *this;
}

int Adhesion::getId() const
{
	return m_id;
}

void Adhesion::setId(int id)
{
	m_id = id;
}

Member& Adhesion::getMember() const
{
	return *m_member;
}

void Adhesion::setMember(const Member &m)
{
	m_member = std::make_shared<Member>(m);
}

int Adhesion::getValue() const
{
	return m_value;
}

void Adhesion::setValue(int value)
{
	m_value = value;
}

QDateTime Adhesion::getOperationDate() const
{
	return m_operationDate;
}

void Adhesion::setOperationDate(QDateTime operationDate)
{
	m_operationDate = operationDate;
}

QDate Adhesion::getBeginDate() const
{
	return m_beginDate;
}

void Adhesion::setBeginDate(QDate beginDate)
{
	m_beginDate = beginDate;
}

QDate Adhesion::getEndDate() const
{
	return m_endDate;
}

void Adhesion::setEndDate(QDate endDate)
{
	m_endDate = endDate;
}

double Adhesion::getPrix() const
{
    return m_prix;
}

void Adhesion::setPrix(double prix)
{
    m_prix = prix;
}