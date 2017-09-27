#ifndef FRANCHISESELECTION_H
#define FRANCHISESELECTION_H

#include <QDialog>
#include "networkadapter.h"
#include "ui_FranchiseSelection.h"

class FranchiseSelection : public QDialog
{
    Q_OBJECT

public:
    FranchiseSelection(int *value, QWidget *parent = 0);
    ~FranchiseSelection();

private slots:
	void feedFranchise(QStringList,QList<QStringList>);
    void franchiseChange(int);
    void validation(QString);

private:
    Ui::FranchiseSelectionClass ui;

    std::unique_ptr<NetworkAdapter> m_networkAdapter;
    int *m_value;
};

#endif // FRANCHISESELECTION_H
