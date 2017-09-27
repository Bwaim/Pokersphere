#ifndef CREATEFIDOPERATION_H
#define CREATEFIDOPERATION_H

#include <QDialog>
#include "Member.h"
#include "networkadapter.h"

namespace Ui {
class CreateFidOperation;
}

class CreateFidOperation : public QDialog
{
    Q_OBJECT

public:
    CreateFidOperation(std::shared_ptr<Member> member, QWidget *parent = 0);
    ~CreateFidOperation();

private slots:
    //void typeFidChanged(int index);
    void cancelCreation(const QString&);
    void saveOperation(const QString&);

private:
    Ui::CreateFidOperation *ui;

    std::shared_ptr<Member> m_member;
    std::unique_ptr<NetworkAdapter> m_networkAdapter;
};

#endif // CREATEFIDOPERATION_H
