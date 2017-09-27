#ifndef CREATEDOTATION_H
#define CREATEDOTATION_H

#include <QDialog>

#include "DotationManager.h"

namespace Ui {
class CreateDotation;
}

class CreateDotation : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDotation(int mode, QWidget *parent = 0);
    ~CreateDotation();

private slots:
    void cancelCreation(const QString&);
    void save(const QString&);

private:
    Ui::CreateDotation *ui;

    int m_mode;
    DotationManager *m_dotationManager;
};

#endif // CREATEDOTATION_H
