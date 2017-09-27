#include "FidHistoric.h"
#include "ui_FidHistoric.h"

#include "constantes.h"
#include "requetes.h"
#include "Transform.h"

#include <QMessageBox>

FidHistoric::FidHistoric(std::shared_ptr<Member> member, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FidHistoric), m_member(member), m_selectedFidOperation(nullptr),
    m_fidOperation(QHash<int, std::shared_ptr<FidOperation>>())
{
    ui->setupUi(this);

    for ( int i = 0 ; i < TAILLE_FID_TYPES ; i++)
        ui->typeFidComboBox->addItem(QString(FID_TYPES_OPERATIONS[i]));

    QStringList args = QStringList();
    args.append(QString::number(m_member->getId()));

    m_networkAdapter.reset(new NetworkAdapter(SELECT_FID_OPERATION_MEMBER,SELECT,args,this));
    connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getFidOperations(QStringList,QList<QStringList>)));
    connect(ui->creerOperationLabel,SIGNAL(linkActivated(QString)),this,SLOT(showCreateFidOperation(QString)));
    connect(ui->enregistrerModificationLabel,SIGNAL(linkActivated(QString)),this,SLOT(updateFidOperation(QString)));
    connect(ui->supprimerOperationLabel,SIGNAL(linkActivated(QString)),this,SLOT(deleteFidOperation(QString)));
}

FidHistoric::~FidHistoric()
{
    delete ui;
}

void FidHistoric::getFidOperations(QStringList headers, QList<QStringList> result)
{
    int row = 0;
    if (m_selectedFidOperation != nullptr && ui->tableView->selectionModel()->selectedRows().size() > 0)
        row = ui->tableView->selectionModel()->selectedRows()[0].row();

    if (ui->tableView->selectionModel() != nullptr)
        delete ui->tableView->selectionModel();
    if (ui->tableView->model() != nullptr)
        delete ui->tableView->model();
    if (m_fidOperation.count() > 0)
    {
        QHash<int, std::shared_ptr<FidOperation>>::iterator i = m_fidOperation.begin();
        while (i != m_fidOperation.end())
        {
            i = m_fidOperation.erase(i);
        }
    }

    m_fidOperation = Transform::toFidOperations(headers,result);
    ui->tableView->setModel(Transform::toModelFidOperationsList(headers,result));
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->resizeColumnsToContents();
    QItemSelectionModel *selectionModel = new QItemSelectionModel(ui->tableView->model());
    ui->tableView->setSelectionModel(selectionModel);
    connect(selectionModel, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));

    if (row < ui->tableView->model()->rowCount())
        ui->tableView->selectRow(row);
    else
        ui->tableView->selectRow(0);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void FidHistoric::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    int id = current.model()->data(current.sibling(current.row(),0)).toInt();
    displayFidOperation(m_fidOperation.value(id));
}
#pragma GCC diagnostic pop

void FidHistoric::displayFidOperation(std::shared_ptr<FidOperation> f)
{
    ui->valeurSpinBox->setValue(f->getValue());
    ui->dateOperationDateTimeEdit->setDateTime(f->getOperationDate());
    ui->typeFidComboBox->setCurrentIndex(f->getType());
    ui->infosAdditionnellesLineEdit->setText(f->getComment());

    m_selectedFidOperation = f;
}

void FidHistoric::showCreateFidOperation(const QString&)
{
    m_createFidOperation.reset(new CreateFidOperation(m_member, this));
    int ret = m_createFidOperation->exec();
    m_createFidOperation.reset();

    if (ret == QDialog::Accepted)
    {
        QStringList args = QStringList();
        args.append(QString::number(m_member->getId()));
        m_networkAdapter.reset(new NetworkAdapter(SELECT_FID_OPERATION_MEMBER,SELECT,args,this));
        connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getFidOperations(QStringList,QList<QStringList>)));
    }
}

void FidHistoric::updateFidOperation(const QString&)
{
    if (m_selectedFidOperation)
    {
        QStringList args = QStringList();
        ui->tableView->selectionModel()->disconnect(SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
        int rowNum = ui->tableView->selectionModel()->selectedRows()[0].row();
        QList<QStandardItem*> row = ((QStandardItemModel*)ui->tableView->model())->takeRow(rowNum);

        args.append(QString::number(m_selectedFidOperation->getId()));

        args.append(QString::number(ui->valeurSpinBox->value()));
        row[1]->setData(QVariant(ui->valeurSpinBox->value()),Qt::DisplayRole);
        m_selectedFidOperation->setValue(ui->valeurSpinBox->value());

        args.append(ui->dateOperationDateTimeEdit->dateTime().toString(QString("dd/MM/yyyy HH:mm")));
        row[2]->setData(QVariant(ui->dateOperationDateTimeEdit->dateTime()),Qt::DisplayRole);
        m_selectedFidOperation->setOperationDate(ui->dateOperationDateTimeEdit->dateTime());

        args.append(QString::number(ui->typeFidComboBox->currentIndex()));
        row[3]->setText(QString(FID_TYPES_OPERATIONS[ui->typeFidComboBox->currentIndex()]));
        m_selectedFidOperation->setType(ui->typeFidComboBox->currentIndex());

        args.append(ui->infosAdditionnellesLineEdit->text());
        row[4]->setText(ui->infosAdditionnellesLineEdit->text());
        m_selectedFidOperation->setComment(ui->infosAdditionnellesLineEdit->text());

        m_networkAdapter.reset(new NetworkAdapter(UPDATE_FID_OPERATION,UPDATE,args,this));

        ((QStandardItemModel*)ui->tableView->model())->insertRow(rowNum,row);
        connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
        ui->tableView->selectRow(rowNum);
    }
}

void FidHistoric::deleteFidOperation(const QString&)
{
    if (m_selectedFidOperation)
    {
        if (QMessageBox::question(this,SUPPRIMER_OPERATION,QString(SUPPRIMER_OPERATION_PHRASE)
            ,QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            QStringList args = QStringList();
            args.append(QString::number(m_selectedFidOperation->getId()));

            m_networkAdapter.reset(new NetworkAdapter(DELETE_FID_OPERATION,DELETE_REQ,args,this));
            int row = ui->tableView->selectionModel()->selectedRows()[0].row();
            ui->tableView->model()->removeRow(row);
            if (row > 0)
                ui->tableView->selectRow(row-1);
            else
                ui->tableView->selectRow(0);
        }
    }
}
