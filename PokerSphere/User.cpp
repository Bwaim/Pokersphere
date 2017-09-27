#include "User.h"

User::User(void)
	: m_id(0), m_login("")
{
	m_userGroup = std::make_shared<UserGroup>();
	m_member = std::make_shared<Member>();
}

User::User(User const& user)
	: m_id(user.m_id), m_login(user.m_login)
{
	m_userGroup = std::make_shared<UserGroup>(*(user.m_userGroup));
	m_member = std::make_shared<Member>(*(user.m_member));
}


User::~User(void)
{
}

User& User::operator=(User const& user)
{
	if (this != &user)
	{
		m_id = user.m_id;
		m_login = user.m_login;
		m_userGroup.reset(new UserGroup(*(user.m_userGroup)));
		m_member.reset(new Member(*(user.m_member)));
	}

	return *this;
}

int User::getId() const
{
	return m_id;
}

void User::setId(int id)
{
	m_id = id;
}

QString User::getLogin() const
{
	return m_login;
}

void User::setLogin(QString login)
{
	m_login = login;
}

std::shared_ptr<UserGroup> User::getIdGroup() const
{
	return m_userGroup;
}

void User::setIdGroup(const UserGroup &idGroup)
{
	m_userGroup.reset(new UserGroup(idGroup));
}

std::shared_ptr<Member> User::getMember() const
{
	return m_member;
}

void User::setMember(Member const& member)
{
	m_member.reset(new Member(member));
}