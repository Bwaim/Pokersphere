#include "DotationMenu.h"
#include "ui_DotationMenu.h"

#include "requetes.h"

DotationMenu::DotationMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DotationMenu), m_dotationManagement(nullptr)
{
    ui->setupUi(this);

    connect(ui->gestionTypeDotationLabel,SIGNAL(linkActivated(QString)),this,SLOT(showDotationTypeManagement(QString)));
    connect(ui->gestionDescriptionDotationLabel,SIGNAL(linkActivated(QString)),this,SLOT(showDotationDescriptionManagement(QString)));
}

DotationMenu::~DotationMenu()
{
    delete ui;
}

void DotationMenu::showDotationTypeManagement(const QString&)
{
    m_dotationManagement.reset(new DotationManagement(DOTATION_TYPE));
    m_dotationManagement->show();
}

void DotationMenu::showDotationDescriptionManagement(const QString&)
{
    m_dotationManagement.reset(new DotationManagement(DOTATION_DESCRIPTION));
    m_dotationManagement->show();
}
