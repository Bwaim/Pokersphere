#include "Kosmos.h"


Kosmos::Kosmos(void)
	: m_member(nullptr)
{
}

Kosmos::Kosmos(Kosmos const& kosmos)
	: m_id(kosmos.m_id), m_value(kosmos.m_value), m_operationDate(kosmos.m_operationDate), m_credit(kosmos.m_credit), m_reason(kosmos.m_reason)
	, m_comment(kosmos.m_comment)
{
	m_member = std::make_shared<Member>(*kosmos.m_member);
}

Kosmos::~Kosmos(void)
{
}

Kosmos& Kosmos::operator=(Kosmos const& kosmos)
{
	if (this != &kosmos)
	{
		m_id = kosmos.m_id;
		m_member = std::make_shared<Member>(*kosmos.m_member);
		m_value = kosmos.m_value;
		m_operationDate = kosmos.m_operationDate;
		m_credit = kosmos.m_credit;
		m_reason = kosmos.m_reason;
		m_comment = kosmos.m_comment;
	}

	return *this;
}

int Kosmos::getId() const
{
	return m_id;
}

void Kosmos::setId(int id)
{
	m_id = id;
}

Member& Kosmos::getMember() const
{
	return *m_member;
}

void Kosmos::setMember(Member const& m)
{
	m_member = std::make_shared<Member>(m);
}

int Kosmos::getValue() const
{
	return m_value;
}

void Kosmos::setValue(int value)
{
	m_value = value;
}

QDateTime Kosmos::getOperationDate() const
{
	return m_operationDate;
}

void Kosmos::setOperationDate(QDateTime operationDate)
{
	m_operationDate = operationDate;
}

bool Kosmos::getCredit() const
{
	return m_credit;
}

void Kosmos::setCredit(bool credit)
{
	m_credit = credit;
}

int Kosmos::getReason() const
{
	return m_reason;
}

void Kosmos::setReason(int reason)
{
	m_reason = reason;
}

void Kosmos::setComment(QString comment)
{
	m_comment = comment;
}

QString Kosmos::getComment() const
{
	return m_comment;
}