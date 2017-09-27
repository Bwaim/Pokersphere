#include "CreateFidStatut.h"
#include "ui_CreateFidStatut.h"

#include "constantes.h"
#include "FidStatut.h"
#include "requetes.h"

#include <QMessageBox>

CreateFidStatut::CreateFidStatut(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateFidStatut)
{
    init();
}

CreateFidStatut::CreateFidStatut(QString name, int seuil, QWidget *parent)
    : QDialog(parent)
{
    init();
    ui->nomLineEdit->setText(name);
    ui->seuilSpinBox->setValue(seuil);
}

void CreateFidStatut::init()
{
    ui->setupUi(this);

    m_fidStatutManager = FidStatutManager::getInstance();

    connect(ui->annulerCreationLabel,SIGNAL(linkActivated(QString)),this,SLOT(cancelCreation(QString)));
    connect(ui->enregistrerFidStatutLabel,SIGNAL(linkActivated(QString)),this,SLOT(saveFidStatut(QString)));
}

CreateFidStatut::~CreateFidStatut()
{
    delete ui;
}

void CreateFidStatut::cancelCreation(const QString&)
{
    reject();
}

void CreateFidStatut::saveFidStatut(const QString&)
{
    std::shared_ptr<FidStatut> fidStatut = std::make_shared<FidStatut>(ui->nomLineEdit->text(),
        ui->seuilSpinBox->value());

    if (!m_fidStatutManager->add(fidStatut,this))
    {
        QMessageBox::warning(this,CREATION_FID_STATUT,QString(NOM_FID_STATUT_EXISTANT));
    }
    else
        m_fidStatutManager->setLastFidSatutAdded(fidStatut);
}
