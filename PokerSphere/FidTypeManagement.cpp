#include "FidTypeManagement.h"
#include "ui_FidTypeManagement.h"

#include "requetes.h"
#include "constantes.h"

#include <QString>
#include <QMessageBox>

FidTypeManagement::FidTypeManagement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FidTypeManagement), m_networkAdapter(nullptr)
{
    ui->setupUi(this);

    m_networkAdapter.reset(new NetworkAdapter(SELECT_FID_TYPES,SELECT,QStringList(),this));
    connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getFidTypes(QStringList,QList<QStringList>)));
    connect(ui->saveLabel, SIGNAL(linkActivated(QString)), this, SLOT(save(QString)));
    connect(ui->closeLabel, SIGNAL(linkActivated(QString)), this, SLOT(close()));
}

FidTypeManagement::~FidTypeManagement()
{
    delete ui;
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void FidTypeManagement::getFidTypes(QStringList headers, QList<QStringList> result)
{
    for (int i = 0; i < TAILLE_FID_TYPES; i++)
    {
        switch (result[i][0].toInt())
        {
            case 0 :
                ui->aRatioDoubleSpinBox->setValue(result[i][2].toFloat());
                ui->aValueSpinBox->setValue(result[i][3].toInt());
                ui->aNbMonthSpinBox->setValue(result[i][4].toInt());
                break;
            case 1 :
                ui->tRatioDoubleSpinBox->setValue(result[i][2].toFloat());
                ui->tValueSpinBox->setValue(result[i][3].toInt());
                ui->tNbMonthSpinBox->setValue(result[i][4].toInt());
                break;
            case 2 :
                ui->eRatioDoubleSpinBox->setValue(result[i][2].toFloat());
                ui->eValueSpinBox->setValue(result[i][3].toInt());
                ui->eNbMonthSpinBox->setValue(result[i][4].toInt());
                break;
            case 3 :
                ui->pRatioDoubleSpinBox->setValue(result[i][2].toFloat());
                ui->pValueSpinBox->setValue(result[i][3].toInt());
                ui->pNbMonthSpinBox->setValue(result[i][4].toInt());
                break;
            case 4 :
                ui->fRatioDoubleSpinBox->setValue(result[i][2].toFloat());
                ui->fValueSpinBox->setValue(result[i][3].toInt());
                ui->fNbMonthSpinBox->setValue(result[i][4].toInt());
                break;
        }
    }
}
#pragma GCC diagnostic pop

void FidTypeManagement::save(const QString&)
{
    QStringList args = QStringList();

    for (int i = 0; i < TAILLE_FID_TYPES; i++)
    {
        args.append(QString::number(i));
        args.append(FID_TYPES[i]);

        switch (i)
        {
            case 0 :
                args.append(QString::number(ui->aRatioDoubleSpinBox->value()));
                args.append(QString::number(ui->aValueSpinBox->value()));
                args.append(QString::number(ui->aNbMonthSpinBox->value()));
                break;
            case 1 :
                args.append(QString::number(ui->tRatioDoubleSpinBox->value()));
                args.append(QString::number(ui->tValueSpinBox->value()));
                args.append(QString::number(ui->tNbMonthSpinBox->value()));
                break;
            case 2 :
                args.append(QString::number(ui->eRatioDoubleSpinBox->value()));
                args.append(QString::number(ui->eValueSpinBox->value()));
                args.append(QString::number(ui->eNbMonthSpinBox->value()));
                break;
            case 3 :
                args.append(QString::number(ui->pRatioDoubleSpinBox->value()));
                args.append(QString::number(ui->pValueSpinBox->value()));
                args.append(QString::number(ui->pNbMonthSpinBox->value()));
                break;
            case 4 :
                args.append(QString::number(ui->fRatioDoubleSpinBox->value()));
                args.append(QString::number(ui->fValueSpinBox->value()));
                args.append(QString::number(ui->fNbMonthSpinBox->value()));
                break;
        }
    }

    m_networkAdapter.reset(new NetworkAdapter(UPDATE_FID_TYPES,UPDATE,args,this));
}
