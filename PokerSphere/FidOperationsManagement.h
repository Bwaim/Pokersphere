#ifndef FIDOPERATIONSMANAGEMENT_H
#define FIDOPERATIONSMANAGEMENT_H

#include <QWidget>
#include <QHash>
#include "networkadapter.h"
#include "Member.h"
#include "FidHistoric.h"

namespace Ui {
class FidOperationsManagement;
}

class FidOperationsManagement : public QWidget
{
    Q_OBJECT

public:
    FidOperationsManagement(QWidget *parent = 0);
    ~FidOperationsManagement();

private slots:
    void getMembers(QStringList headers, QList<QStringList> result);
    void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void showFidOperationsHistoric(const QString&);
    void updatePhoto(QStringList infos, QString);
    void doubleClicked(const QModelIndex &current);
    void updateFidPointsMember(QStringList headers, QList<QStringList> result);

private:
    void displayMember(std::shared_ptr<Member> m, int abonnement);

    Ui::FidOperationsManagement *ui;

    std::shared_ptr<Member> m_selectedMember;
    QHash<int,std::shared_ptr<Member>> m_member;
    std::unique_ptr<NetworkAdapter> m_networkAdapter;
    std::unique_ptr<NetworkAdapter> m_networkAdapterPhoto;
    std::unique_ptr<NetworkAdapter> m_networkAdapterFid;
    std::unique_ptr<FidHistoric> m_fidHistoric;
};

#endif // FIDOPERATIONSMANAGEMENT_H
