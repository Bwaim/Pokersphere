#include "DotationManagement.h"
#include "ui_DotationManagement.h"

#include "Transform.h"
#include "requetes.h"

#include <QMessageBox>

DotationManagement::DotationManagement(int mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DotationManagement), m_createDotation(nullptr), m_selectedDotation(""), m_mode(mode)
{
    ui->setupUi(this);

    m_dotationManager = DotationManager::getInstance();

    if (m_mode == DOTATION_TYPE)
    {
        ui->detailDotationLabel->setText("Détail du Type de dotation");
    }
    else if (m_mode == DOTATION_DESCRIPTION)
    {
        ui->detailDotationLabel->setText("Détail de la description de dotation");
        ui->ajouterDotationLabel->setText("<a href=\"a\" style=\"text-decoration:none;\"><font color=\"#5E2F6A\">Ajouter une description</font></a>");
        ui->supprimerDotationLabel->setText("<a href=\"a\" style=\"text-decoration:none;\"><font color=\"#5E2F6A\">Supprimer la description</font></a>");
    }

    connect(ui->ajouterDotationLabel,SIGNAL(linkActivated(QString)),this,SLOT(showCreateDotation(QString)));
    connect(ui->enregistrerModificationLabel,SIGNAL(linkActivated(QString)),this,SLOT(updateDotation(QString)));
    connect(ui->supprimerDotationLabel,SIGNAL(linkActivated(QString)),this,SLOT(deleteDotation(QString)));
    connect(m_dotationManager,SIGNAL(isLoaded(void)),this,SLOT(getDotations(void)));

    getDotations();
}

DotationManagement::~DotationManagement()
{
    delete ui;
}

void DotationManagement::getDotations()
{
    int row = 0;
    if (m_selectedDotation != "" && ui->tableView->selectionModel()->selectedRows().size() > 0)
        row = ui->tableView->selectionModel()->selectedRows()[0].row();


    if (ui->tableView->selectionModel() != nullptr)
        delete ui->tableView->selectionModel();
    if (ui->tableView->model() != nullptr)
        delete ui->tableView->model();

    ui->tableView->setModel(Transform::toModelDotation(m_dotationManager,m_mode));
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
void DotationManagement::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QString name = current.model()->data(current.sibling(current.row(),0)).toString();
    displayDotation(name);
}
#pragma GCC diagnostic pop

void DotationManagement::displayDotation(QString dotation)
{
    ui->nomLineEdit->setText(dotation);

    m_selectedDotation = dotation;
}

void DotationManagement::updateDotation(const QString&)
{
    bool modified = false;
    if (!m_selectedDotation.isEmpty())
    {
        if (m_mode == DOTATION_TYPE)
        {
            if ( m_dotationManager->modifyType(ui->nomLineEdit->text(), m_selectedDotation, this))
                modified = true;
            else
                QMessageBox::warning(this,INFOS_DOTATION,QString(DOTATION_TYPE_EXISTANT));
        }
        else if (m_mode == DOTATION_DESCRIPTION)
        {
            if ( m_dotationManager->modifyDescription(ui->nomLineEdit->text(), m_selectedDotation, this))
                modified = true;
            else
                QMessageBox::warning(this,INFOS_DOTATION,QString(DOTATION_DESCRIPTION_EXISTANT));
        }

        if (modified)
        {
            ui->tableView->selectionModel()->disconnect(SIGNAL(currentRowChanged(QModelIndex, QModelIndex))
                , this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
            int rowNum = ui->tableView->selectionModel()->selectedRows()[0].row();
            QList<QStandardItem*> row = ((QStandardItemModel*)ui->tableView->model())->takeRow(rowNum);

            row[0]->setText(ui->nomLineEdit->text());

            ((QStandardItemModel*)ui->tableView->model())->insertRow(rowNum,row);
            connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex))
                , this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
            ui->tableView->selectRow(rowNum);
        }
    }
    else
        QMessageBox::warning(this,INFOS_DOTATION,QString(INFOS_DOTATION_PHRASE));
}

void DotationManagement::showCreateDotation(const QString&)
{
    m_createDotation.reset(new CreateDotation(m_mode,this));
    m_createDotation->exec();
    m_createDotation.reset();

    getDotations();
}

void DotationManagement::deleteDotation(const QString&)
{
    if (!m_selectedDotation.isEmpty())
    {
        if (QMessageBox::question(this,SUPP_DOTATION,QString(SUPP_DOTATION_PHRASE)
            + m_selectedDotation + QString(" ?")
            ,QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            if (m_mode == DOTATION_TYPE)
            {
                m_dotationManager->removeType(m_selectedDotation, this);
            }
            else if (m_mode == DOTATION_DESCRIPTION)
            {
                m_dotationManager->removeDescription(m_selectedDotation, this);
            }

            disconnect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex))
                , this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));

            int row = ui->tableView->selectionModel()->selectedRows()[0].row();
            ui->tableView->model()->removeRow(row);
            connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex))
                , this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
            if (row > 0)
            {
                ui->tableView->selectRow(row-1);
                m_selectedDotation = ui->tableView->selectionModel()->selectedRows(1)[0].data(Qt::DisplayRole).toString();
            }
            else
                if (ui->tableView->model()->rowCount() > 0)
                {
                    ui->tableView->selectRow(0);
                    m_selectedDotation = ui->tableView->selectionModel()->selectedRows(1)[0].data(Qt::DisplayRole).toString();
                }
                else
                    m_selectedDotation = "";
        }
    }
    else
        QMessageBox::warning(this,INFOS_DOTATION,QString(INFOS_DOTATION_PHRASE));
}
