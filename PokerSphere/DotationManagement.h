#ifndef DOTATIONMANAGEMENT_H
#define DOTATIONMANAGEMENT_H

#include <QWidget>

#include "CreateDotation.h"
#include "DotationManager.h"

namespace Ui {
class DotationManagement;
}

class DotationManagement : public QWidget
{
    Q_OBJECT

public:
    explicit DotationManagement(int mode, QWidget *parent = 0);
    ~DotationManagement();

private slots:
    void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void showCreateDotation(const QString&);
    void updateDotation(const QString&);
    void deleteDotation(const QString&);
    void getDotations();

private:
    void displayDotation(QString dotation);

    Ui::DotationManagement *ui;

    DotationManager *m_dotationManager;
    std::unique_ptr<CreateDotation> m_createDotation;
    QString m_selectedDotation;
    int m_mode;
};

#endif // DOTATIONMANAGEMENT_H
