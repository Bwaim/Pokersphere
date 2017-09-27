#ifndef FIDSTATUTMANAGEMENT_H
#define FIDSTATUTMANAGEMENT_H

#include <QWidget>
#include "CreateFidStatut.h"
#include "FidStatutManager.h"
#include "FidStatut.h"

namespace Ui {
class FidStatutManagement;
}

class FidStatutManagement : public QWidget
{
    Q_OBJECT

public:
    explicit FidStatutManagement(QWidget *parent = 0);
    ~FidStatutManagement();

private slots:
    void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void showCreateFidStatut(const QString&);
    void updateFidStatut(const QString&);
    void deleteFidStatut(const QString&);
    void getFidStatuts();

private:
    void displayFidStatut(std::shared_ptr<FidStatut> fidStatut);

    Ui::FidStatutManagement *ui;

    FidStatutManager *m_fidStatutManager;
    std::unique_ptr<CreateFidStatut> m_createFidStatut;
    std::shared_ptr<FidStatut> m_selectedFidStatut;
};

#endif // FIDSTATUTMANAGEMENT_H
