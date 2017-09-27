#ifndef FIDTYPEMANAGEMENT_H
#define FIDTYPEMANAGEMENT_H

#include <QWidget>
#include "networkadapter.h"

namespace Ui {
class FidTypeManagement;
}

class FidTypeManagement : public QWidget
{
    Q_OBJECT

public:
    FidTypeManagement(QWidget *parent = 0);
    ~FidTypeManagement();

private slots:
    void getFidTypes(QStringList headers, QList<QStringList> result);
    void save(const QString&);

private:
    Ui::FidTypeManagement *ui;

    std::unique_ptr<NetworkAdapter> m_networkAdapter;
};

#endif // FIDTYPEMANAGEMENT_H
