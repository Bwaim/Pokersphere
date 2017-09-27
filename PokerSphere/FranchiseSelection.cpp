#include "FranchiseSelection.h"

#include "requetes.h"
#include <QMessageBox>

FranchiseSelection::FranchiseSelection(int *value, QWidget *parent)
    : QDialog(parent), m_networkAdapter(nullptr), m_value(value)
{
    ui.setupUi(this);

    m_networkAdapter.reset(new NetworkAdapter(SELECT_FRANCHISE,SELECT,QStringList(),this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(feedFranchise(QStringList, QList<QStringList>)));
    connect(ui.validerLabel,SIGNAL(linkActivated(QString)),this,SLOT(validation(QString)));
    connect(ui.franchiseComboBox,SIGNAL(activated(int)),this,SLOT(franchiseChange(int)));
}

FranchiseSelection::~FranchiseSelection()
{

}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void FranchiseSelection::feedFranchise(QStringList headers, QList<QStringList> result)
{
    ui.franchiseComboBox->addItem(QString(""));
    for (int i =0; i < result.size(); i++)
        ui.franchiseComboBox->addItem(result[i][1]);
}
#pragma GCC diagnostic pop

void FranchiseSelection::franchiseChange(int value)
{
    *m_value = value;
}

void FranchiseSelection::validation(QString)
{
    if (*m_value > 0)
        accept();
    else
        QMessageBox::warning(this,QString("Attention"),QString("Vous devez choisir une franchise"));
}
