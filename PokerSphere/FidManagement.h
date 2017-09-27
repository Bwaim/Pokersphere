#ifndef FIDMANAGEMENT_H
#define FIDMANAGEMENT_H

#include <QWidget>
#include "FidStatutManagement.h"
#include "FidTypeManagement.h"

namespace Ui {
class FidManagement;
}

class FidManagement : public QWidget
{
    Q_OBJECT

public:
    explicit FidManagement(QWidget *parent = 0);
    ~FidManagement();

private slots:
    void showFidStatutManagement(const QString&);
    void showFidTypeManagement(const QString&);

private:
    Ui::FidManagement *ui;

    std::unique_ptr<FidStatutManagement> m_fidStatutManagement;
    std::unique_ptr<FidTypeManagement> m_fidTypeManagement;
};

#endif // FIDMANAGEMENT_H
