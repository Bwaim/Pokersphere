#ifndef FIDHISTORIC_H
#define FIDHISTORIC_H

#include <QDialog>
#include "Member.h"
#include "networkadapter.h"
#include "FidOperation.h"
#include "CreateFidOperation.h"

namespace Ui {
class FidHistoric;
}

class FidHistoric : public QDialog
{
    Q_OBJECT

public:
    FidHistoric(std::shared_ptr<Member> member, QWidget *parent = 0);
    ~FidHistoric();

private slots:
    void getFidOperations(QStringList headers, QList<QStringList> result);
    void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void showCreateFidOperation(const QString&);
    void updateFidOperation(const QString&);
    void deleteFidOperation(const QString&);

private:
    void displayFidOperation(std::shared_ptr<FidOperation> f);

    Ui::FidHistoric *ui;

    std::shared_ptr<Member> m_member;
    std::shared_ptr<FidOperation> m_selectedFidOperation;
    std::unique_ptr<NetworkAdapter> m_networkAdapter;
    QHash<int,std::shared_ptr<FidOperation>> m_fidOperation;
    std::unique_ptr<CreateFidOperation> m_createFidOperation;
};

#endif // FIDHISTORIC_H
