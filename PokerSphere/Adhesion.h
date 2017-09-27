#ifndef ADHESION_H
#define ADHESION_H

#include "Member.h"
#include <QDate>
#include <QDateTime>

class Adhesion
{
public:
	Adhesion(void);
	Adhesion(Adhesion const& adhesion);
	~Adhesion(void);

	Adhesion& operator=(Adhesion const& adhesion);

	int getId() const;
	void setId(int id);
	Member& getMember() const;
	void setMember(Member const& m);
	int getValue() const;
	void setValue(int value);
	QDateTime getOperationDate() const;
	void setOperationDate(QDateTime operationDate);
	QDate getBeginDate() const;
	void setBeginDate(QDate beginDate);
	QDate getEndDate() const;
	void setEndDate(QDate endDate);
    double getPrix() const;
	void setPrix(double prix);

private:
	int m_id;
	std::shared_ptr<Member> m_member;
	int m_value;
	QDateTime m_operationDate;
	QDate m_beginDate;
	QDate m_endDate;
    double m_prix;
};

#endif // ADHESION_H