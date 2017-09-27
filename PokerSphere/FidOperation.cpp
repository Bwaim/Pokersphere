#include "FidOperation.h"

FidOperation::FidOperation()
    : m_member(nullptr)
{

}

FidOperation::FidOperation(FidOperation const& fidOperation)
    : m_id(fidOperation.m_id), m_value(fidOperation.m_value), m_operationDate(fidOperation.m_operationDate)
    , m_type(fidOperation.m_type)
    , m_comment(fidOperation.m_comment)
{
    m_member = std::make_shared<Member>(*fidOperation.m_member);
}

FidOperation::~FidOperation(void)
{
}

FidOperation& FidOperation::operator=(FidOperation const& fidOperation)
{
    if (this != &fidOperation)
    {
        m_id = fidOperation.m_id;
        m_member = std::make_shared<Member>(*fidOperation.m_member);
        m_value = fidOperation.m_value;
        m_operationDate = fidOperation.m_operationDate;
        m_type = fidOperation.m_type;
        m_comment = fidOperation.m_comment;
    }

    return *this;
}

int FidOperation::getId() const
{
    return m_id;
}

void FidOperation::setId(int id)
{
    m_id = id;
}

Member& FidOperation::getMember() const
{
    return *m_member;
}

void FidOperation::setMember(Member const& m)
{
    m_member = std::make_shared<Member>(m);
}

int FidOperation::getValue() const
{
    return m_value;
}

void FidOperation::setValue(int value)
{
    m_value = value;
}

QDateTime FidOperation::getOperationDate() const
{
    return m_operationDate;
}

void FidOperation::setOperationDate(QDateTime operationDate)
{
    m_operationDate = operationDate;
}

int FidOperation::getType() const
{
    return m_type;
}

void FidOperation::setType(int type)
{
    m_type = type;
}

void FidOperation::setComment(QString comment)
{
    m_comment = comment;
}

QString FidOperation::getComment() const
{
    return m_comment;
}
