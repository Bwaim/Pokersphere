#ifndef USER_H
#define USER_H

#include <QString>
#include "UserGroup.h"
#include "Member.h"

class User
{
public:
	User(void);
	User(User const& user);
	~User(void);

	User& operator=(User const& user);

	int getId() const;
	void setId(int id);
	QString getLogin() const;
	void setLogin(QString login);
	std::shared_ptr<UserGroup> getIdGroup() const;
	void setIdGroup(UserGroup const& idGroup);
	std::shared_ptr<Member> getMember() const;
	void setMember(Member const& member);

private:
	int m_id;
	QString m_login;
	std::shared_ptr<UserGroup> m_userGroup;
	std::shared_ptr<Member> m_member;
};

#endif // USER_H