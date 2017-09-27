#include "ClubManagement.h"
#include "ui_ClubManagement.h"

#include "Transform.h"
#include "requetes.h"
#include "Parameter.h"
#include <QMessageBox>

ClubManagement::ClubManagement(int franchise, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClubManagement), m_createClub(nullptr), m_selectedClub(), m_franchise(franchise)
{
    ui->setupUi(this);

    setWindowTitle(QString("Gestion des clubs ") + Parameter::getInstance()->getFranchiseLabel(m_franchise));

    m_clubManager = ClubManager::getInstance(m_franchise);

    connect(ui->ajouterClubLabel,SIGNAL(linkActivated(QString)),this,SLOT(showCreateClub(QString)));
    connect(ui->enregistrerModificationLabel,SIGNAL(linkActivated(QString)),this,SLOT(updateClub(QString)));
    connect(ui->supprimerClubLabel,SIGNAL(linkActivated(QString)),this,SLOT(deleteClub(QString)));
    connect(m_clubManager,SIGNAL(isLoaded(void)),this,SLOT(getClubs(void)));

    getClubs();
}

ClubManagement::~ClubManagement()
{
    delete ui;
}

void ClubManagement::getClubs()
{
    int row = 0;
    if (m_selectedClub != nullptr && ui->tableView->selectionModel()->selectedRows().size() > 0)
        row = ui->tableView->selectionModel()->selectedRows()[0].row();


    if (ui->tableView->selectionModel() != nullptr)
        delete ui->tableView->selectionModel();
    if (ui->tableView->model() != nullptr)
        delete ui->tableView->model();

    ui->tableView->setModel(Transform::toModelClubs(m_clubManager));
    //ui->tableView->setColumnHidden(0,true);
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
void ClubManagement::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QString club = current.model()->data(current.sibling(current.row(),0)).toString();
    displayClub(club);
}
#pragma GCC diagnostic pop

void ClubManagement::displayClub(QString club)
{
    ui->clubLineEdit->setText(club);

    m_selectedClub = club;
}

void ClubManagement::updateClub(const QString&)
{
    if (!m_selectedClub.isEmpty())
    {
        QString newClub = ui->clubLineEdit->text();

        if ( m_clubManager->modify(newClub, m_selectedClub, this))
        {
            ui->tableView->selectionModel()->disconnect(SIGNAL(currentRowChanged(QModelIndex, QModelIndex))
                , this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
            int rowNum = ui->tableView->selectionModel()->selectedRows()[0].row();
            QList<QStandardItem*> row = ((QStandardItemModel*)ui->tableView->model())->takeRow(rowNum);

            row[0]->setText(ui->clubLineEdit->text());

            ((QStandardItemModel*)ui->tableView->model())->insertRow(rowNum,row);
            connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex))
                , this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
            ui->tableView->selectRow(rowNum);
        }
        else
            QMessageBox::warning(this,INFOS_CLUBS,QString(CLUB_EXISTANT));

    }
    else
        QMessageBox::warning(this,INFOS_CLUBS,QString(INFOS_CLUBS_PHRASE));
}

void ClubManagement::showCreateClub(const QString&)
{
    m_createClub.reset(new CreateClub(m_franchise, this));
    m_createClub->exec();
    m_createClub.reset();

    getClubs();
}

void ClubManagement::deleteClub(const QString&)
{
    if (!m_selectedClub.isEmpty())
    {
        if (QMessageBox::question(this,SUPP_CLUB,QString(SUPP_CLUB_PHRASE)
            + m_selectedClub + QString(" ?")
            ,QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            m_clubManager->remove(m_selectedClub, this);

            disconnect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex))
                , this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));

            int row = ui->tableView->selectionModel()->selectedRows()[0].row();
            ui->tableView->model()->removeRow(row);
            if (row > 0)
            {
                ui->tableView->selectRow(row-1);
                QString name = ui->tableView->selectionModel()->selectedRows(0)[0].data(Qt::DisplayRole).toString();
                m_selectedClub = name;
            }
            else
                if (ui->tableView->model()->rowCount() > 0)
                {
                    ui->tableView->selectRow(0);
                    QString name = ui->tableView->selectionModel()->selectedRows(0)[0].data(Qt::DisplayRole).toString();
                    m_selectedClub = name;
                }
                else
                    m_selectedClub.clear();

            connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex))
                , this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
        }

    }
    else
        QMessageBox::warning(this,INFOS_CLUBS,QString(INFOS_CLUBS_PHRASE));
}
