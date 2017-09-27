#include "membership.h"
#include "requetes.h"
#include "Transform.h"
#include <QMessageBox>

Membership::Membership(std::shared_ptr<Member> m, QWidget *parent)
	: QDialog(parent), m_member(m), m_selectedAdhesion(nullptr), m_adhesion(QHash<int, std::shared_ptr<Adhesion>>())
{
	ui.setupUi(this);

	m_membershipManager = MembershipManager::getInstance();
	ui.typeAbonnementComboBox->addItems(m_membershipManager->getDisplayList());
	ui.typeAbonnementComboBox->setCurrentIndex(1);

	QStringList args = QStringList();
	args.append(QString::number(m_member->getId()));

	connect(ui.typeAbonnementComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(typeAbonnementChanged(int)));

	m_networkAdapter.reset(new NetworkAdapter(SELECT_MEMBERSHIP_MEMBER,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMemberships(QStringList, QList<QStringList>)));
	connect(ui.creerAdhesionLabel,SIGNAL(linkActivated(QString)),this,SLOT(showCreateAdhesion(QString)));
	connect(ui.enregistrerModificationLabel,SIGNAL(linkActivated(QString)),this,SLOT(updateAdhesion(QString)));
	connect(ui.supprimerAdhesionLabel,SIGNAL(linkActivated(QString)),this,SLOT(deleteAdhesion(QString)));
}

Membership::~Membership()
{
}

void Membership::getMemberships(QStringList headers, QList<QStringList> result)
{
	int row = 0;
	if (m_selectedAdhesion != nullptr && ui.tableView->selectionModel()->selectedRows().size() > 0)
		row = ui.tableView->selectionModel()->selectedRows()[0].row();

	if (ui.tableView->selectionModel() != nullptr)
		delete ui.tableView->selectionModel();
	if (ui.tableView->model() != nullptr)
		delete ui.tableView->model();
	if (m_adhesion.count() > 0)
	{
		QHash<int, std::shared_ptr<Adhesion>>::iterator i = m_adhesion.begin();
		while (i != m_adhesion.end())
		{
			//delete i.value();
			i = m_adhesion.erase(i);
		}
	}

	m_adhesion = Transform::toAdhesions(headers,result);
	ui.tableView->setModel(Transform::toModelAdhesions(headers,result));
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
void Membership::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
	int id = current.model()->data(current.sibling(current.row(),0)).toInt();
	displayAdhesion(m_adhesion.value(id));
}
#pragma GCC diagnostic pop

void Membership::displayAdhesion(std::shared_ptr<Adhesion> a)
{
	ui.typeAbonnementComboBox->setCurrentIndex(m_membershipManager->getDisplayIndex(a->getValue()));
	
	ui.dateOperationDateTimeEdit->setDateTime(a->getOperationDate());
	ui.debutAbonnementDateEdit->setDate(a->getBeginDate());
	ui.finAbonnementDateEdit->setDate(a->getEndDate());
    ui.prixDoubleSpinBox->setValue(a->getPrix());

	m_selectedAdhesion = a;
}

void Membership::showCreateAdhesion(const QString&)
{
	int nbRows = ui.tableView->model()->rowCount();
    QDate endDate = QDate::currentDate();
    if (nbRows > 0)
    {
	    int id = ((QStandardItemModel*)ui.tableView->model())->item(nbRows-1)->text().toInt();
	    endDate = m_adhesion.value(id)->getEndDate();
    }
	m_createAdhesion.reset(new CreateAdhesion(m_member, endDate, this));
	int ret = m_createAdhesion->exec();
    m_createAdhesion.reset();
	
	if (ret == QDialog::Accepted)
	{
		QStringList args = QStringList();
		args.append(QString::number(m_member->getId()));
		m_networkAdapter.reset(new NetworkAdapter(SELECT_MEMBERSHIP_MEMBER,SELECT,args,this));
		connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMemberships(QStringList, QList<QStringList>)));
	}
}

void Membership::updateAdhesion(const QString&)
{
	QStringList args = QStringList();
	ui.tableView->selectionModel()->disconnect(SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
	int rowNum = ui.tableView->selectionModel()->selectedRows()[0].row();
	QList<QStandardItem*> row = ((QStandardItemModel*)ui.tableView->model())->takeRow(rowNum);
	
	args.append(QString::number(m_selectedAdhesion->getId()));

	args.append(QString::number(m_membershipManager->getValue(ui.typeAbonnementComboBox->currentText())));
	row[1]->setText(ui.typeAbonnementComboBox->currentText());
	m_selectedAdhesion->setValue(m_membershipManager->getValue(ui.typeAbonnementComboBox->currentText()));
	
	
	args.append(ui.dateOperationDateTimeEdit->dateTime().toString(QString("dd/MM/yyyy HH:mm")));
	//row[2]->setText(ui.dateOperationDateTimeEdit->dateTime().toString(QString("dd/MM/yyyy HH:mm")));
	row[2]->setData(QVariant(ui.dateOperationDateTimeEdit->dateTime()),Qt::DisplayRole);
	m_selectedAdhesion->setOperationDate(ui.dateOperationDateTimeEdit->dateTime());
	
	args.append(ui.debutAbonnementDateEdit->date().toString(QString("dd/MM/yyyy")));
	//row[3]->setText(ui.debutAbonnementDateEdit->date().toString(QString("dd/MM/yyyy")));
	row[3]->setData(QVariant(ui.debutAbonnementDateEdit->date()),Qt::DisplayRole);
	m_selectedAdhesion->setBeginDate(ui.debutAbonnementDateEdit->date());
	
	args.append(ui.finAbonnementDateEdit->date().toString(QString("dd/MM/yyyy")));
	//row[4]->setText(ui.finAbonnementDateEdit->date().toString(QString("dd/MM/yyyy")));
	row[4]->setData(QVariant(ui.finAbonnementDateEdit->date()),Qt::DisplayRole);
	m_selectedAdhesion->setEndDate(ui.finAbonnementDateEdit->date());

    args.append(QString::number(ui.prixDoubleSpinBox->value(),'f',2));
    row[5]->setText(QString::number(ui.prixDoubleSpinBox->value(),'f',2));
    m_selectedAdhesion->setPrix(ui.prixDoubleSpinBox->value());

    args.append(QString::number(m_membershipManager->getNbMonth(ui.typeAbonnementComboBox->currentText())));
	
	m_networkAdapter.reset(new NetworkAdapter(UPDATE_ADHESION,UPDATE,args,this));

	((QStandardItemModel*)ui.tableView->model())->insertRow(rowNum,row);
	connect(ui.tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
	ui.tableView->selectRow(rowNum);
}

void Membership::deleteAdhesion(const QString&)
{
	if (QMessageBox::question(this,SUPP_ADHESION,QString(SUPP_ADHESION_PHRASE) 
		+ m_selectedAdhesion->getBeginDate().toString(QString("dd/MM/yyyy")) + QString(" au ") 
		+ m_selectedAdhesion->getEndDate().toString(QString("dd/MM/yyyy")) + QString(" ?")
		,QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
	{
		QStringList args = QStringList();
		args.append(QString::number(m_selectedAdhesion->getId()));

		m_networkAdapter.reset(new NetworkAdapter(DELETE_ADHESION,DELETE_REQ,args,this));
		int row = ui.tableView->selectionModel()->selectedRows()[0].row();
		ui.tableView->model()->removeRow(row);
		if (row > 0)
			ui.tableView->selectRow(row-1);
		else
			ui.tableView->selectRow(0);
	}
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void Membership::typeAbonnementChanged(int index)
{
	ui.finAbonnementDateEdit->setDate(m_membershipManager->getEndDate(ui.typeAbonnementComboBox->currentText()
		,ui.debutAbonnementDateEdit->date()));
}
#pragma GCC diagnostic pop
