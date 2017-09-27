#include "TournamentTemplateManagement.h"

#include "constantes.h"
#include "Transform.h"
#include "requetes.h"

#include <QMessageBox>

TournamentTemplateManagement::TournamentTemplateManagement(int franchise, QWidget *parent)
    : QWidget(parent),m_createTournamentTemplate(nullptr), m_selectedTournamentTemplate(nullptr), m_franchise(franchise)
{
    ui.setupUi(this);

    m_tournamentTemplateManager = TournamentTemplateManager::getInstance(franchise);

    for (int i = 0 ; i < TAILLE_VARIANTE ; i++)
		ui.varianteComboBox->addItem(QString(TYPE_VARIANTE[i]));

	connect(ui.ajouterTemplateTournoiLabel,SIGNAL(linkActivated(QString)),this,SLOT(showCreateTournamentTemplate(QString)));
	connect(ui.enregistrerModificationLabel,SIGNAL(linkActivated(QString)),this,SLOT(updateTournamentTemplate(QString)));
	connect(ui.supprimerTemplateTournoiLabel,SIGNAL(linkActivated(QString)),this,SLOT(deleteTournamentTemplate(QString)));
    connect(ui.dupliquerLabel,SIGNAL(linkActivated(QString)),this,SLOT(dupliquerTournamentTemplate(QString)));
    connect(m_tournamentTemplateManager,SIGNAL(isLoaded(void)),this,SLOT(getTournamentsTemplate(void)));

    getTournamentsTemplate();
}

TournamentTemplateManagement::~TournamentTemplateManagement()
{

}

void TournamentTemplateManagement::getTournamentsTemplate()
{
    int row = 0;
	if (m_selectedTournamentTemplate != nullptr && ui.tableView->selectionModel()->selectedRows().size() > 0)
		row = ui.tableView->selectionModel()->selectedRows()[0].row();

	
	if (ui.tableView->selectionModel() != nullptr)
		delete ui.tableView->selectionModel();
	if (ui.tableView->model() != nullptr)
		delete ui.tableView->model();

	ui.tableView->setModel(Transform::toModelTournamentsTemplate(m_tournamentTemplateManager));
	ui.tableView->setColumnHidden(0,true);
	ui.tableView->resizeColumnsToContents();
	QItemSelectionModel *selectionModel = new QItemSelectionModel(ui.tableView->model());
	ui.tableView->setSelectionModel(selectionModel);
	connect(selectionModel, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));

	if (row < ui.tableView->model()->rowCount())
		ui.tableView->selectRow(row);
	else
		ui.tableView->selectRow(0);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void TournamentTemplateManagement::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
	QString name = current.model()->data(current.sibling(current.row(),1)).toString();
	displayTournamentTemplate(m_tournamentTemplateManager->get(name));
}
#pragma GCC diagnostic pop

void TournamentTemplateManagement::displayTournamentTemplate(std::shared_ptr<TournamentTemplate> tournamentTemplate)
{
	ui.nomLineEdit->setText(tournamentTemplate->getName());
	ui.buyInSpinBox->setValue(tournamentTemplate->getBuyIn());
	ui.nombreJoueursSpinBox->setValue(tournamentTemplate->getNbPlayers());
	ui.varianteComboBox->setCurrentIndex(tournamentTemplate->getVariante());
	ui.commentLineEdit->setText(tournamentTemplate->getComment());

    m_selectedTournamentTemplate = tournamentTemplate;
}

void TournamentTemplateManagement::updateTournamentTemplate(const QString&)
{
	if (m_selectedTournamentTemplate)
	{
        TournamentTemplate newTournamentTemplate = TournamentTemplate(ui.nomLineEdit->text(),ui.buyInSpinBox->value()
            ,ui.nombreJoueursSpinBox->value(),ui.varianteComboBox->currentIndex(), ui.commentLineEdit->text());

        if ( m_tournamentTemplateManager->modify(newTournamentTemplate, m_selectedTournamentTemplate, this))
        {
            ui.tableView->selectionModel()->disconnect(SIGNAL(currentRowChanged(QModelIndex, QModelIndex))
                , this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
            int rowNum = ui.tableView->selectionModel()->selectedRows()[0].row();
            QList<QStandardItem*> row = ((QStandardItemModel*)ui.tableView->model())->takeRow(rowNum);

            row[1]->setText(ui.nomLineEdit->text());
            row[2]->setData(QVariant(ui.buyInSpinBox->value()),Qt::DisplayRole);
            row[3]->setData(QVariant(ui.nombreJoueursSpinBox->value()),Qt::DisplayRole);
            row[4]->setText(ui.varianteComboBox->currentText());
            row[5]->setText(ui.commentLineEdit->text());

            ((QStandardItemModel*)ui.tableView->model())->insertRow(rowNum,row);
            connect(ui.tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex))
                , this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
            ui.tableView->selectRow(rowNum);
        }
        else
		    QMessageBox::warning(this,INFOS_TEMPLATE_TOURNOI,QString(NOM_TEMPLATE_EXISTANT));
        
	}
	else
		QMessageBox::warning(this,INFOS_TEMPLATE_TOURNOI,QString(INFOS_TEMPLATE_TOURNOI_PHRASE));
}

void TournamentTemplateManagement::showCreateTournamentTemplate(const QString&)
{
	m_createTournamentTemplate.reset(new CreateTournamentTemplate(m_franchise, this));
    m_createTournamentTemplate->exec();
    m_createTournamentTemplate.reset();
	
    getTournamentsTemplate();
}

void TournamentTemplateManagement::dupliquerTournamentTemplate(const QString&)
{
    if (m_selectedTournamentTemplate)
	{
        m_createTournamentTemplate.reset(new CreateTournamentTemplate(ui.nomLineEdit->text(), ui.buyInSpinBox->value()
            ,ui.nombreJoueursSpinBox->value(), ui.varianteComboBox->currentIndex(), ui.commentLineEdit->text(), m_franchise));
        m_createTournamentTemplate->exec();
        m_createTournamentTemplate.reset();
	    
        getTournamentsTemplate();
    }
    else
        QMessageBox::warning(this,INFOS_TEMPLATE_TOURNOI,QString(INFOS_TEMPLATE_TOURNOI_PHRASE));
}

void TournamentTemplateManagement::deleteTournamentTemplate(const QString&)
{
	if (m_selectedTournamentTemplate)
	{
        if (QMessageBox::question(this,SUPP_TEMPLATE_TOURNOI,QString(SUPP_TEMPLATE_TOURNOI_PHRASE) 
            + m_selectedTournamentTemplate->getName() + QString(" ?")
            ,QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            m_tournamentTemplateManager->remove(m_selectedTournamentTemplate->getName(), this);

            disconnect(ui.tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex))
                , this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));

            int row = ui.tableView->selectionModel()->selectedRows()[0].row();
            ui.tableView->model()->removeRow(row);
            connect(ui.tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex))
                , this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
            if (row > 0)
            {
                ui.tableView->selectRow(row-1);
                QString name = ui.tableView->selectionModel()->selectedRows(1)[0].data(Qt::DisplayRole).toString();
	            m_selectedTournamentTemplate = m_tournamentTemplateManager->get(name);
            }
            else
                if (ui.tableView->model()->rowCount() > 0)
                {
                    ui.tableView->selectRow(0);
                    QString name = ui.tableView->selectionModel()->selectedRows(1)[0].data(Qt::DisplayRole).toString();
	                m_selectedTournamentTemplate = m_tournamentTemplateManager->get(name);
                }
                else
                    m_selectedTournamentTemplate = nullptr;

        }

	}
	else
		QMessageBox::warning(this,INFOS_TEMPLATE_TOURNOI,QString(INFOS_TEMPLATE_TOURNOI_PHRASE));
}
