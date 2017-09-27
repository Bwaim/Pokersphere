#include "UserGroup.h"


UserGroup::UserGroup(void)
	: m_id(0), m_name("")
{
}

UserGroup::UserGroup(UserGroup const& userGroup)
	: m_id(userGroup.m_id), m_name(userGroup.m_name)
{
}

UserGroup::UserGroup(int id, QString name)
	: m_id(id), m_name(name)
{
}


UserGroup::~UserGroup(void)
{
}

UserGroup& UserGroup::operator=(UserGroup const& userGroup)
{
	if (this != &userGroup)
	{
		m_id = userGroup.m_id;
		m_name = userGroup.m_name;
	}

	return *this;
}

int UserGroup::getId() const
{
	return m_id;
}

void UserGroup::setId(int id)
{
	m_id = id;
}

QString UserGroup::getName() const
{
	return m_name;
}

void UserGroup::setName(QString name)
{
	m_name = name;
}