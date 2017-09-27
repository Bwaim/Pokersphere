#ifndef KOSMOS_H
#define KOSMOS_H

#include <QString>
#include <QDateTime>
#include "Member.h"

class Kosmos
{
public:
	Kosmos(void);
	Kosmos(Kosmos const& kosmos);
	~Kosmos(void);

	Kosmos& operator=(Kosmos const& kosmos);

	int getId() const;
	void setId(int id);
	Member& getMember() const;
	void setMember(Member const& m);
	int getValue() const;
	void setValue(int value);
	QDateTime getOperationDate() const;
	void setOperationDate(QDateTime operationDate);
	bool getCredit() const;
	void setCredit(bool credit);
	int getReason() const;
	void setReason(int reason);
	void setComment(QString comment);
	QString getComment() const;

private:
	int m_id;
	std::shared_ptr<Member> m_member;
	int m_value;
	QDateTime m_operationDate;
	bool m_credit;
	int m_reason;
	QString m_comment;
};

#endif // KOSMOS_H