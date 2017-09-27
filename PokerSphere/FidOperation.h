#ifndef FIDOPERATION_H
#define FIDOPERATION_H

#include <QString>
#include <QDateTime>
#include "Member.h"

class FidOperation
{
public:
    FidOperation();
    FidOperation(FidOperation const& fidOperation);
    ~FidOperation(void);

    FidOperation& operator=(FidOperation const& fidOperation);

    int getId() const;
    void setId(int id);
    Member& getMember() const;
    void setMember(Member const& m);
    int getValue() const;
    void setValue(int value);
    QDateTime getOperationDate() const;
    void setOperationDate(QDateTime operationDate);
    int getType() const;
    void setType(int type);
    void setComment(QString comment);
    QString getComment() const;

private:
    int m_id;
    std::shared_ptr<Member> m_member;
    int m_value;
    QDateTime m_operationDate;
    int m_type;
    QString m_comment;
};

#endif // FIDOPERATION_H
