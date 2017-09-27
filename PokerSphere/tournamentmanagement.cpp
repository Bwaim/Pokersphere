#include "tournamentmanagement.h"
#include "requetes.h"
#include "constantes.h"
#include "Transform.h"
#include "Parameter.h"

#include <QMessageBox>
#include <QPainter>

TournamentManagement::TournamentManagement(int typeTournament, QWidget *parent)
	: m_createTournament(nullptr), m_selectedTournament(nullptr), m_networkAdapter(nullptr), m_tournamentInformations(nullptr)
	, m_presentValidated(true), m_typeTournament(typeTournament), m_mainWindow(parent), m_networkAdapterFranchise(nullptr), m_networkAdapterTournamentList(nullptr)
{
	ui.setupUi(this);

	ui.filtreDebutDateEdit->setDate(QDate::currentDate());
	ui.filtreFinDateEdit->setDate(QDate::currentDate().addMonths(1));

	for (int i = 0 ; i < TAILLE_VARIANTE ; i++)
		ui.varianteComboBox->addItem(QString(TYPE_VARIANTE[i]));

    if (m_typeTournament == 1)
    {
        this->setWindowTitle(QString("Gestion des Sit & Go"));
        ui.detailTournoiLabel->setText(QString("Détail du Sit & Go :"));
        ui.ajouterTournoiLabel->setText(QString("<a href=\"a\" style=\"text-decoration:none;\"><font color=\"#5E2F6A\">Ajouter un Sit & Go</font></a>"));
        ui.dupliquerLabel->setText(QString("<a href=\"a\" style=\"text-decoration:none;\"><font color=\"#5E2F6A\">Dupliquer un Sit & Go</font></a>"));
        ui.supprimerTournoiLabel->setText(QString("<a href=\"a\" style=\"text-decoration:none;\"><font color=\"#5E2F6A\">Supprimer le Sit & Go</font></a>"));
    }

	connect(ui.dateDebutDateTimeEdit, SIGNAL(dateTimeChanged(QDateTime)), ui.dateFinInscriptionDateTimeEdit, SLOT(setDateTime(QDateTime)));
	connect(ui.filtrerPushButton, SIGNAL(clicked(bool)), this, SLOT(doFilter(bool)));
	connect(ui.ajouterTournoiLabel,SIGNAL(linkActivated(QString)),this,SLOT(showCreateTournament(QString)));
	connect(ui.enregistrerModificationLabel,SIGNAL(linkActivated(QString)),this,SLOT(updateTournament(QString)));
	connect(ui.supprimerTournoiLabel,SIGNAL(linkActivated(QString)),this,SLOT(deleteTournament(QString)));
	connect(ui.listeInscritsLabel,SIGNAL(linkActivated(QString)),this,SLOT(showTournamentSubscribes(QString)));
	connect(ui.detailLabel,SIGNAL(linkActivated(QString)),this,SLOT(showTournamentInformations(QString)));
    connect(ui.dupliquerLabel,SIGNAL(linkActivated(QString)),this,SLOT(duplicateTournament(QString)));
    connect(ui.tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClicked(QModelIndex)));
    doFilter(true);

    Parameter *param = Parameter::getInstance();
    int franchise = param->getFranchise();

    if (franchise == 0)
    {
        m_networkAdapterFranchise.reset(new NetworkAdapter(SELECT_FRANCHISE,SELECT,QStringList(),this));
	    connect(m_networkAdapterFranchise.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(feedFranchise(QStringList, QList<QStringList>)));
    }
    else
        ui.franchiseComboBox->setHidden(true);
}

TournamentManagement::~TournamentManagement()
{
	while (!m_presentValidated)
		QMessageBox::warning(this,QString(ATTENTE),(ATTENTE_PHRASE));
}

QWidget* TournamentManagement::getMainWindow()
{
    return m_mainWindow;
}

void TournamentManagement::doFilter(bool)
{
	QStringList args = QStringList();
	args.append(ui.filtreDebutDateEdit->date().toString(QString("dd/MM/yyyy")));
	args.append(ui.filtreFinDateEdit->date().toString(QString("dd/MM/yyyy")));
    args.append(QString::number(m_typeTournament));

    Parameter *param = Parameter::getInstance();
    int franchise = param->getFranchise();
    if (franchise == 0)
    {
        franchise = ui.franchiseComboBox->currentIndex();
        args.append(QString::number(franchise));
    }

	m_networkAdapter.reset(new NetworkAdapter(SELECT_TOURNAMENT,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getTournaments(QStringList, QList<QStringList>)));
}

void TournamentManagement::getTournaments(QStringList headers, QList<QStringList> result)
{
	m_presentValidated = true;

	int row = 0;
	if (m_selectedTournament != nullptr && ui.tableView->selectionModel()->selectedRows().size() > 0)
		row = ui.tableView->selectionModel()->selectedRows()[0].row();

	
	if (ui.tableView->selectionModel() != nullptr)
		delete ui.tableView->selectionModel();
	if (ui.tableView->model() != nullptr)
		delete ui.tableView->model();
	if (m_tournament.count() > 0)
	{
		QHash<int, std::shared_ptr<Tournament>>::iterator i = m_tournament.begin();
		while (i != m_tournament.end())
		{
			//delete i.value();
			i = m_tournament.erase(i);
		}
	}

	m_tournament = Transform::toTournaments(headers,result);
	ui.tableView->setModel(Transform::toModelTournaments(headers,result));
	ui.tableView->setColumnHidden(0,true);
    //VALIDATED
	ui.tableView->setColumnHidden(headers.size()-3,true);
    //SENT_TO_RANKINGHERO
    ui.tableView->setColumnHidden(headers.size()-4,true);
    //FRANCHISE
	ui.tableView->setColumnHidden(headers.size()-2,true);
    //ATTACHED_TOURNAMENT
	ui.tableView->setColumnHidden(headers.size()-1,true);
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
void TournamentManagement::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
	int id = current.model()->data(current.sibling(current.row(),0)).toInt();

    QStringList args = QStringList();
	args.append(QString::number(m_tournament.value(id)->getFranchise()));
    args.append(m_tournament.value(id)->getBeginDate().toString(QString("dd/MM/yyyy")));
    args.append(QString::number(m_tournament.value(id)->getId()));
    args.append(QString::number(m_typeTournament));
    m_networkAdapterFranchise.reset(new NetworkAdapter(SELECT_TOURNAMENT_ASSOCIATED_WITH_DATE,SELECT,args,this));
	connect(m_networkAdapterFranchise.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(feedTournamentList(QStringList, QList<QStringList>)));

	displayTournament(m_tournament.value(id));
}
#pragma GCC diagnostic pop

void TournamentManagement::doubleClicked(const QModelIndex &current)
{
    int id = current.model()->data(current.sibling(current.row(),0)).toInt();
    m_selectedTournament = m_tournament.value(id);
    showTournamentSubscribes(QString());
}

void TournamentManagement::displayTournament(std::shared_ptr<Tournament> t)
{
	ui.nomLineEdit->setText(t->getName());
	ui.dateDebutDateTimeEdit->setDateTime(t->getBeginDate());
	ui.dateFinInscriptionDateTimeEdit->setDateTime(t->getEndRegistrationDate());
	ui.buyInSpinBox->setValue(t->getBuyIn());
	ui.nombreJoueursSpinBox->setValue(t->getNbPlayers());
	ui.varianteComboBox->setCurrentIndex(t->getVariante());
	ui.nombreInscritsSpinBox->setValue(t->getNbSubscribes());
	ui.commentLineEdit->setText(t->getComment());
    ui.privateCheckBox->setCheckState(t->getPrivate() ? Qt::Checked : Qt::Unchecked);

	m_selectedTournament = t;
}

void TournamentManagement::updateTournament(const QString&)
{
	if (m_selectedTournament)
	{
		if ( !m_selectedTournament->getIsValidated())
		{
			QStringList args = QStringList();
			ui.tableView->selectionModel()->disconnect(SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
			int rowNum = ui.tableView->selectionModel()->selectedRows()[0].row();
			QList<QStandardItem*> row = ((QStandardItemModel*)ui.tableView->model())->takeRow(rowNum);

			args.append(QString::number(m_selectedTournament->getId()));

			args.append(ui.nomLineEdit->text());
			row[1]->setText(ui.nomLineEdit->text());
			m_selectedTournament->setName(ui.nomLineEdit->text());

			args.append(ui.dateDebutDateTimeEdit->dateTime().toString(QString("dd/MM/yyyy HH:mm")));
			//row[2]->setText(ui.dateDebutDateTimeEdit->dateTime().toString(QString("dd/MM/yyyy HH:mm")));
			row[2]->setData(QVariant(ui.dateDebutDateTimeEdit->dateTime()),Qt::DisplayRole);
			m_selectedTournament->setBeginDate(ui.dateDebutDateTimeEdit->dateTime());

			args.append(ui.dateFinInscriptionDateTimeEdit->dateTime().toString(QString("dd/MM/yyyy HH:mm")));
			//row[3]->setText(ui.dateFinInscriptionDateTimeEdit->dateTime().toString(QString("dd/MM/yyyy HH:mm")));
			row[3]->setData(QVariant(ui.dateFinInscriptionDateTimeEdit->dateTime()),Qt::DisplayRole);
			m_selectedTournament->setEndRegistrationDate(ui.dateFinInscriptionDateTimeEdit->dateTime());

			args.append(QString::number(ui.buyInSpinBox->value()));
			//row[4]->setText(QString::number(ui.buyInSpinBox->value()));
			row[4]->setData(QVariant(ui.buyInSpinBox->value()),Qt::DisplayRole);
			m_selectedTournament->setBuyIn(ui.buyInSpinBox->value());

			args.append(QString::number(ui.nombreJoueursSpinBox->value()));
			//row[5]->setText(QString::number(ui.nombreJoueursSpinBox->value()));
			row[5]->setData(QVariant(ui.nombreJoueursSpinBox->value()),Qt::DisplayRole);
			m_selectedTournament->setNbPlayers(ui.nombreJoueursSpinBox->value());

			args.append(QString::number(ui.varianteComboBox->currentIndex()));
			row[6]->setText(ui.varianteComboBox->currentText());
			m_selectedTournament->setVariante(ui.varianteComboBox->currentIndex());

			args.append(ui.commentLineEdit->text());
			row[9]->setText(ui.commentLineEdit->text());
			m_selectedTournament->setComment(ui.commentLineEdit->text());

            args.append(QString::number(ui.privateCheckBox->checkState() == Qt::Unchecked ? 0 : 1));
			row[8]->setText(ui.privateCheckBox->checkState() == Qt::Unchecked ? NON : OUI);
			m_selectedTournament->setPrivate(ui.privateCheckBox->checkState() == Qt::Unchecked ? false : true);

            args.append(QString::number(ui.tournoiLieComboBox->itemData(ui.tournoiLieComboBox->currentIndex()).toInt()));
            row[13]->setText(ui.tournoiLieComboBox->itemData(ui.tournoiLieComboBox->currentIndex()).toString());
            m_selectedTournament->setAttachedTournament(ui.tournoiLieComboBox->itemData(ui.tournoiLieComboBox->currentIndex()).toInt());

			m_networkAdapter.reset(new NetworkAdapter(UPDATE_TOURNAMENT,UPDATE,args,this));

			((QStandardItemModel*)ui.tableView->model())->insertRow(rowNum,row);
			connect(ui.tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
			ui.tableView->selectRow(rowNum);
		}
		else
		{
			QMessageBox::warning(this,MODIFIER_TOURNOI,QString(MODIFIER_TOURNOI_PHRASE));
		}
	}
	else
		QMessageBox::warning(this,INFOS_TOURNOI,QString(INFOS_TOURNOI_PHRASE));
}

void TournamentManagement::showCreateTournament(const QString&)
{
	m_createTournament.reset(new CreateTournament(m_typeTournament, this));
	int ret = m_createTournament->exec();
	m_createTournament.reset();

	if (ret == QDialog::Accepted)
	{
		doFilter(true);
	}
}

void TournamentManagement::deleteTournament(const QString&)
{
	if (m_selectedTournament)
	{
		if ( !m_selectedTournament->getIsValidated())
		{
			if (QMessageBox::question(this,SUPP_TOURNOI,QString(SUPP_TOURNOI_PHRASE) 
				+ m_selectedTournament->getName() + QString(" du ") + m_selectedTournament->getBeginDate().toString(QString("dd/MM/yyyy HH:mm")) + QString(" ?")
				,QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
			{
				QStringList args = QStringList();
				args.append(QString::number(m_selectedTournament->getId()));

				m_networkAdapter.reset(new NetworkAdapter(DELETE_TOURNAMENT,DELETE_REQ,args,this));

				disconnect(ui.tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));

				int row = ui.tableView->selectionModel()->selectedRows()[0].row();
				ui.tableView->model()->removeRow(row);
				if (row > 0)
					ui.tableView->selectRow(row-1);
				else
					if (ui.tableView->model()->rowCount() > 0)
						ui.tableView->selectRow(0);
					else
						m_selectedTournament = nullptr;

				connect(ui.tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
			}
		}
		else
		{
			QMessageBox::warning(this,MODIFIER_TOURNOI,QString(MODIFIER_TOURNOI_PHRASE));
		}
	}
	else
		QMessageBox::warning(this,INFOS_TOURNOI,QString(INFOS_TOURNOI_PHRASE));
}

void TournamentManagement::showTournamentSubscribes(const QString&)
{
	if (m_tournament.size() > 0)
	{
		//Delete à la fermeture de la fenêtre avec setAttribute(Qt::WA_DeleteOnClose);
        TournamentDetail * tournamentDetail = new TournamentDetail(m_selectedTournament, this);
        tournamentDetail->show();
	}
}

void TournamentManagement::showTournamentInformations(const QString&)
{
	if (m_selectedTournament)
	{
		m_tournamentInformations.reset(new TournamentInformations(m_selectedTournament));
		m_tournamentInformations->show();
	}
	else
		QMessageBox::warning(this,INFOS_TOURNOI,QString(INFOS_TOURNOI_PHRASE));
}

void TournamentManagement::duplicateTournament(const QString&)
{
    if (m_selectedTournament)
	{
		m_createTournament.reset(new CreateTournament(ui.nomLineEdit->text(), ui.buyInSpinBox->value(), ui.nombreJoueursSpinBox->value()
            , ui.varianteComboBox->currentIndex(), ui.commentLineEdit->text(), ui.privateCheckBox->isChecked(), m_typeTournament, ui.dateDebutDateTimeEdit->dateTime()
            , ui.dateFinInscriptionDateTimeEdit->dateTime(), ui.tournoiLieComboBox->itemData(ui.tournoiLieComboBox->currentIndex()).toInt(), this));
        int ret = m_createTournament->exec();
        m_createTournament.reset();

        if (ret == QDialog::Accepted)
        {
	        doFilter(true);
        }
	}
	else
		QMessageBox::warning(this,INFOS_TOURNOI,QString(INFOS_TOURNOI_PHRASE));
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void TournamentManagement::feedFranchise(QStringList headers, QList<QStringList> result)
{
    ui.franchiseComboBox->addItem(QString("France"));
    for (int i =0; i < result.size(); i++)
        ui.franchiseComboBox->addItem(result[i][1]);
}
#pragma GCC diagnostic pop

#pragma GCC diagnostic ignored "-Wunused-parameter"
void TournamentManagement::feedTournamentList(QStringList headers, QList<QStringList> result)
{
    ui.tournoiLieComboBox->clear();
    ui.tournoiLieComboBox->addItem("", -1);
    if (m_selectedTournament->getAttachedTournament() == -1)
        ui.tournoiLieComboBox->setCurrentIndex(0);
    for (int i = 0 ; i < result.size() ; i++)
    {
        ui.tournoiLieComboBox->addItem(result[i][1], result[i][0].toInt());
        if (m_selectedTournament->getAttachedTournament() == result[i][0].toInt())
            ui.tournoiLieComboBox->setCurrentIndex(i+1);
    }
}
#pragma GCC diagnostic pop
