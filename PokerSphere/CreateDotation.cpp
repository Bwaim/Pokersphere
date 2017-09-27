#include "CreateDotation.h"
#include "ui_CreateDotation.h"

#include "requetes.h"

#include <QMessageBox>

CreateDotation::CreateDotation(int mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDotation), m_mode(mode)
{
    ui->setupUi(this);

    m_dotationManager = DotationManager::getInstance();

    if (m_mode == DOTATION_TYPE)
    {
        setWindowTitle("Création d'un type de dotation");
        ui->creationDotationLabel->setText(QString("Création d'un type de dotation"));
        ui->enregistrerDotationLabel->setText(QString("<a href=\"a\" style=\"text-decoration:none;\"><font color=\"#5E2F6A\">Enregistrer le type</font></a>"));
    }
    else if (m_mode == DOTATION_DESCRIPTION)
    {
        setWindowTitle("Création d'une description de dotation");
        ui->creationDotationLabel->setText(QString("Création d'une description de dotation"));
        ui->enregistrerDotationLabel->setText(QString("<a href=\"a\" style=\"text-decoration:none;\"><font color=\"#5E2F6A\">Enregistrer la description</font></a>"));
    }

    connect(ui->annulerCreationLabel,SIGNAL(linkActivated(QString)),this,SLOT(cancelCreation(QString)));
    connect(ui->enregistrerDotationLabel,SIGNAL(linkActivated(QString)),this,SLOT(save(QString)));
}

CreateDotation::~CreateDotation()
{
    delete ui;
}

void CreateDotation::cancelCreation(const QString&)
{
    reject();
}

void CreateDotation::save(const QString&)
{
    if (m_mode == DOTATION_TYPE)
    {
        if (!m_dotationManager->addType(ui->nomLineEdit->text(),this))
        {
            QMessageBox::warning(this,CREATION_DOTATION_TYPE,QString(DOTATION_TYPE_EXISTANT));
        }
    }
    else if (m_mode == DOTATION_DESCRIPTION)
    {
        if (!m_dotationManager->addDescription(ui->nomLineEdit->text(),this))
        {
            QMessageBox::warning(this,CREATION_DOTATION_DESCRPITION,QString(DOTATION_DESCRIPTION_EXISTANT));
        }
    }

}
