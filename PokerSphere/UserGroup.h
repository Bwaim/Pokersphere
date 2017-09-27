#ifndef USERGROUP_H
#define USERGROUP_H

#include <QString>

class UserGroup
{
public:
	UserGroup(void);
	UserGroup(int id, QString name);
	UserGroup(UserGroup const& userGroup);
	~UserGroup(void);

	UserGroup& operator=(UserGroup const& userGroup);

	int getId() const;
	void setId(int id);
	QString getName() const;
	void setName(QString name);

private:
	int m_id;
	QString m_name;
};

#endif // USERGROUP_H