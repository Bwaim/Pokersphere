#include "CreateFidOperation.h"
#include "ui_CreateFidOperation.h"
#include "constantes.h"
#include "requetes.h"

CreateFidOperation::CreateFidOperation(std::shared_ptr<Member> member, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateFidOperation), m_member(member), m_networkAdapter(nullptr)
{
    ui->setupUi(this);

    ui->dateOperationDateTimeEdit->setDateTime(QDateTime::currentDateTime());

    for ( int i = 0 ; i < TAILLE_FID_TYPES ; i++)
        ui->typeFidComboBox->addItem(QString(FID_TYPES_OPERATIONS[i]));

    //connect(ui->typeFidComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(typeFidChanged(int)));
    connect(ui->annulerCreationLabel,SIGNAL(linkActivated(QString)),this,SLOT(cancelCreation(QString)));
    connect(ui->enregistrerOperationLabel,SIGNAL(linkActivated(QString)),this,SLOT(saveOperation(QString)));
}

CreateFidOperation::~CreateFidOperation()
{
    delete ui;
}

/*void CreateFidOperation::typeFidChanged(int index)
{
    ui->valeurSpinBox->setEnabled(true);
    if (index == 1)
    {
        ui->valeurSpinBox->setEnabled(false);
        ui->valeurSpinBox->setValue(1);
    }
}*/

void CreateFidOperation::cancelCreation(const QString&)
{
    reject();
}

void CreateFidOperation::saveOperation(const QString&)
{
    QStringList args = QStringList();
    args.append(QString::number(m_member->getId()));
    args.append(ui->dateOperationDateTimeEdit->dateTime().toString(QString("dd/MM/yyyy HH:mm")));
    int type = ui->typeFidComboBox->currentIndex();
    QString typeStr = QString();
    //if (type !=4 TAILLE_FID_TYPES-1)
    typeStr = QString::number(type);
    args.append(typeStr);
    args.append(QString::number(ui->valeurSpinBox->value()));
    args.append(ui->infosAdditionnellesLineEdit->text());

    m_networkAdapter.reset(new NetworkAdapter(CREATE_FID_OPERATION,INSERT,args,this));

    connect(m_networkAdapter.get(), SIGNAL(finished()), this ,SLOT(accept()));
}
