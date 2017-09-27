#include "FidStatutManagement.h"
#include "ui_FidStatutManagement.h"

#include "constantes.h"
#include "Transform.h"
#include "requetes.h"

#include <QMessageBox>

FidStatutManagement::FidStatutManagement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FidStatutManagement),m_createFidStatut(nullptr), m_selectedFidStatut(nullptr)
{
    ui->setupUi(this);

    m_fidStatutManager = FidStatutManager::getInstance();

    connect(ui->ajouterFidStatutLabel,SIGNAL(linkActivated(QString)),this,SLOT(showCreateFidStatut(QString)));
    connect(ui->enregistrerModificationLabel,SIGNAL(linkActivated(QString)),this,SLOT(updateFidStatut(QString)));
    connect(ui->supprimerFidStatutLabel,SIGNAL(linkActivated(QString)),this,SLOT(deleteFidStatut(QString)));
    connect(m_fidStatutManager,SIGNAL(isLoaded(void)),this,SLOT(getFidStatuts(void)));

    getFidStatuts();
}

FidStatutManagement::~FidStatutManagement()
{
    delete ui;
}

void FidStatutManagement::getFidStatuts()
{
    int row = 0;
    if (m_selectedFidStatut != nullptr && ui->tableView->selectionModel()->selectedRows().size() > 0)
        row = ui->tableView->selectionModel()->selectedRows()[0].row();


    if (ui->tableView->selectionModel() != nullptr)
        delete ui->tableView->selectionModel();
    if (ui->tableView->model() != nullptr)
        delete ui->tableView->model();

    ui->tableView->setModel(Transform::toModelFidStatuts(m_fidStatutManager));
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->resizeColumnsToContents();
    QItemSelectionModel *selectionModel = new QItemSelectionModel(ui->tableView->model());
    ui->tableView->setSelectionModel(selectionModel);
    connect(selectionModel, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));

    if (row < ui->tableView->model()->rowCount())
        ui->tableView->selectRow(row);
    else
        ui->tableView->selectRow(0);

    ui->tableView->sortByColumn(2,Qt::DescendingOrder);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void FidStatutManagement::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QString name = current.model()->data(current.sibling(current.row(),1)).toString();
    displayFidStatut(m_fidStatutManager->get(name));
}
#pragma GCC diagnostic pop

void FidStatutManagement::displayFidStatut(std::shared_ptr<FidStatut> fidStatut)
{
    ui->nomLineEdit->setText(fidStatut->getName());
    ui->seuilSpinBox->setValue(fidStatut->getSeuil());

    m_selectedFidStatut = fidStatut;
}

void FidStatutManagement::updateFidStatut(const QString&)
{
    if (m_selectedFidStatut)
    {
        FidStatut newFidStatut = FidStatut(ui->nomLineEdit->text(),ui->seuilSpinBox->value());

        if ( m_fidStatutManager->modify(newFidStatut, m_selectedFidStatut, this))
        {
            ui->tableView->selectionModel()->disconnect(SIGNAL(currentRowChanged(QModelIndex, QModelIndex))
                , this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
            int rowNum = ui->tableView->selectionModel()->selectedRows()[0].row();
            QList<QStandardItem*> row = ((QStandardItemModel*)ui->tableView->model())->takeRow(rowNum);

            row[1]->setText(ui->nomLineEdit->text());
            row[2]->setData(QVariant(ui->seuilSpinBox->value()),Qt::DisplayRole);

            ((QStandardItemModel*)ui->tableView->model())->insertRow(rowNum,row);
            connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex))
                , this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
            ui->tableView->selectRow(rowNum);
        }
        else
            QMessageBox::warning(this,INFOS_FID_STATUT,QString(NOM_FID_STATUT_EXISTANT));

    }
    else
        QMessageBox::warning(this,INFOS_FID_STATUT,QString(INFOS_FID_STATUT_PHRASE));
}

void FidStatutManagement::showCreateFidStatut(const QString&)
{
    m_createFidStatut.reset(new CreateFidStatut(this));
    m_createFidStatut->exec();
    m_createFidStatut.reset();

    getFidStatuts();
}

void FidStatutManagement::deleteFidStatut(const QString&)
{
    if (m_selectedFidStatut)
    {
        if (QMessageBox::question(this,SUPP_FID_STATUT,QString(SUPP_FID_STATUT_PHRASE)
            + m_selectedFidStatut->getName() + QString(" ?")
            ,QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            m_fidStatutManager->remove(m_selectedFidStatut->getName(), this);

            disconnect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex))
                , this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));

            int row = ui->tableView->selectionModel()->selectedRows()[0].row();
            ui->tableView->model()->removeRow(row);
            connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex))
                , this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
            if (row > 0)
            {
                ui->tableView->selectRow(row-1);
                QString name = ui->tableView->selectionModel()->selectedRows(1)[0].data(Qt::DisplayRole).toString();
                m_selectedFidStatut = m_fidStatutManager->get(name);
            }
            else
                if (ui->tableView->model()->rowCount() > 0)
                {
                    ui->tableView->selectRow(0);
                    QString name = ui->tableView->selectionModel()->selectedRows(1)[0].data(Qt::DisplayRole).toString();
                    m_selectedFidStatut = m_fidStatutManager->get(name);
                }
                else
                    m_selectedFidStatut = nullptr;

        }

    }
    else
        QMessageBox::warning(this,INFOS_FID_STATUT,QString(INFOS_FID_STATUT_PHRASE));
}

