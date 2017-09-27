#ifndef EUROSLOADING_H
#define EUROSLOADING_H

#include <QDialog>
#include "Member.h"
#include "networkadapter.h"

namespace Ui {
class EurosLoading;
}

class EurosLoading : public QDialog
{
    Q_OBJECT

public:
    EurosLoading(std::shared_ptr<Member> member, QWidget *parent = 0);
    ~EurosLoading();

private slots:
    void cancelDepot(const QString&);
    void saveDepot(const QString&);
    void updatePhoto(QStringList infos, QString);

private:
    Ui::EurosLoading *ui;

    std::shared_ptr<Member> m_member;
    std::unique_ptr<NetworkAdapter> m_networkAdapter;
};

#endif // EUROSLOADING_H
