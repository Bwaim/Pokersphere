#include "FidManagement.h"
#include "ui_FidManagement.h"

FidManagement::FidManagement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FidManagement), m_fidStatutManagement(nullptr), m_fidTypeManagement(nullptr)
{
    ui->setupUi(this);

    connect(ui->manageRankLabel,SIGNAL(linkActivated(QString)),this,SLOT(showFidStatutManagement(QString)));
    connect(ui->manageTypesLabel,SIGNAL(linkActivated(QString)),this,SLOT(showFidTypeManagement(QString)));
}

FidManagement::~FidManagement()
{
    delete ui;
}

void FidManagement::showFidStatutManagement(const QString&)
{
    m_fidStatutManagement.reset(new FidStatutManagement());
    m_fidStatutManagement->show();
}

void FidManagement::showFidTypeManagement(const QString&)
{
    m_fidTypeManagement.reset(new FidTypeManagement());
    m_fidTypeManagement->show();
}
