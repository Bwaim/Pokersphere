#include "kosmoshistoric.h"
#include "constantes.h"
#include "requetes.h"
#include "Transform.h"
#include <QMessageBox>

KosmosHistoric::KosmosHistoric(std::shared_ptr<Member> member,QWidget *parent)
	: QDialog(parent), m_member(member), m_selectedKosmos(nullptr), m_kosmos(QHash<int, std::shared_ptr<Kosmos>>()), m_kosmosExtraction(nullptr)
{
	ui.setupUi(this);

	for ( int i = 0 ; i < TAILLE_TYPE_PARTIE_COMPLET ; i++)
		ui.typeOperationComboBox->addItem(QString(TYPE_PARTIE_COMPLET[i]));

	QStringList args = QStringList();
	args.append(QString::number(m_member->getId()));

	m_networkAdapter.reset(new NetworkAdapter(SELECT_KOSMOS_MEMBER,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getKosmos(QStringList, QList<QStringList>)));
	connect(ui.creerOperationLabel,SIGNAL(linkActivated(QString)),this,SLOT(showCreateKosmos(QString)));
	connect(ui.enregistrerModificationLabel,SIGNAL(linkActivated(QString)),this,SLOT(updateKosmos(QString)));
	connect(ui.supprimerOperationLabel,SIGNAL(linkActivated(QString)),this,SLOT(deleteKosmos(QString)));
    connect(ui.extraireLabel,SIGNAL(linkActivated(QString)),this,SLOT(extractKosmos(QString)));
}

KosmosHistoric::~KosmosHistoric()
{
}

void KosmosHistoric::getKosmos(QStringList headers, QList<QStringList> result)
{
	int row = 0;
	if (m_selectedKosmos != nullptr && ui.tableView->selectionModel()->selectedRows().size() > 0)
		row = ui.tableView->selectionModel()->selectedRows()[0].row();

	if (ui.tableView->selectionModel() != nullptr)
		delete ui.tableView->selectionModel();
	if (ui.tableView->model() != nullptr)
		delete ui.tableView->model();
	if (m_kosmos.count() > 0)
	{
		QHash<int, std::shared_ptr<Kosmos>>::iterator i = m_kosmos.begin();
		while (i != m_kosmos.end())
		{
			//delete i.value();
			i = m_kosmos.erase(i);
		}
	}

	m_kosmos = Transform::toKosmos(headers,result);
	ui.tableView->setModel(Transform::toModelKosmosList(headers,result));
	ui.tableView->setColumnHidden(0,true);
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
void KosmosHistoric::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
	int id = current.model()->data(current.sibling(current.row(),0)).toInt();
	displayKosmos(m_kosmos.value(id));
}
#pragma GCC diagnostic pop

void KosmosHistoric::displayKosmos(std::shared_ptr<Kosmos> k)
{
	if (k->getCredit())
		ui.creditRadioButton->setChecked(true);
	else
		ui.debitRadioButton->setChecked(true);

	ui.valeurSpinBox->setValue(k->getValue());
	ui.dateOperationDateTimeEdit->setDateTime(k->getOperationDate());
	ui.typeOperationComboBox->setCurrentIndex(k->getReason());
	ui.infosAdditionnellesLineEdit->setText(k->getComment());
	
	m_selectedKosmos = k;
}

void KosmosHistoric::showCreateKosmos(const QString&)
{
	m_createKosmosOperation.reset(new CreateKosmosOperation(m_member, this));
	int ret = m_createKosmosOperation->exec();
	m_createKosmosOperation.reset();

	if (ret == QDialog::Accepted)
	{
		QStringList args = QStringList();
		args.append(QString::number(m_member->getId()));
		m_networkAdapter.reset(new NetworkAdapter(SELECT_KOSMOS_MEMBER,SELECT,args,this));
		connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getKosmos(QStringList, QList<QStringList>)));
	}
}

void KosmosHistoric::updateKosmos(const QString&)
{
	QStringList args = QStringList();
	ui.tableView->selectionModel()->disconnect(SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
	int rowNum = ui.tableView->selectionModel()->selectedRows()[0].row();
	QList<QStandardItem*> row = ((QStandardItemModel*)ui.tableView->model())->takeRow(rowNum);
	
	args.append(QString::number(m_selectedKosmos->getId()));

	QString icon = ui.creditRadioButton->isChecked() ? QString(":/img/Resources/flecheup.gif") : QString(":/img/Resources/flechedown.gif");

	args.append(QString::number(ui.valeurSpinBox->value()));
	//row[1]->setText(QString("%L1").arg(ui.valeurSpinBox->value()));
	row[1]->setData(QVariant(ui.valeurSpinBox->value()),Qt::DisplayRole);
	row[1]->setIcon(QIcon(QPixmap(icon)));
	int old_value = m_selectedKosmos->getValue();
	m_selectedKosmos->setValue(ui.valeurSpinBox->value());
	
	args.append(ui.dateOperationDateTimeEdit->dateTime().toString(QString("dd/MM/yyyy HH:mm")));
	//row[2]->setText(ui.dateOperationDateTimeEdit->dateTime().toString(QString("dd/MM/yyyy HH:mm")));
	row[2]->setData(QVariant(ui.dateOperationDateTimeEdit->dateTime()),Qt::DisplayRole);
	m_selectedKosmos->setOperationDate(ui.dateOperationDateTimeEdit->dateTime());
	
	args.append(QString(ui.creditRadioButton->isChecked() ? UN : ZERO));
	row[5]->setText(QString(ui.creditRadioButton->isChecked() ? OUI : ""));
	bool old_credit = m_selectedKosmos->getCredit();
	m_selectedKosmos->setCredit(ui.creditRadioButton->isChecked());

	args.append(QString::number(ui.typeOperationComboBox->currentIndex()));
	row[3]->setText(QString(TYPE_PARTIE_COMPLET[ui.typeOperationComboBox->currentIndex()]));
	m_selectedKosmos->setReason(ui.typeOperationComboBox->currentIndex());
	
	args.append(ui.infosAdditionnellesLineEdit->text());
	row[4]->setText(ui.infosAdditionnellesLineEdit->text());
	m_selectedKosmos->setComment(ui.infosAdditionnellesLineEdit->text());
	
	args.append(QString::number(m_member->getId()));

	args.append(QString::number((old_credit ? -1 * old_value : old_value )
		+ (ui.creditRadioButton->isChecked() ? ui.valeurSpinBox->value() : -1 * ui.valeurSpinBox->value())));
	
	m_networkAdapter.reset(new NetworkAdapter(UPDATE_KOSMOS_OPERATION,UPDATE,args,this));

	((QStandardItemModel*)ui.tableView->model())->insertRow(rowNum,row);
	connect(ui.tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
	ui.tableView->selectRow(rowNum);
}

void KosmosHistoric::deleteKosmos(const QString&)
{
	if (QMessageBox::question(this,SUPPRIMER_OPERATION,QString(SUPPRIMER_OPERATION_PHRASE) 
		,QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
	{
		QStringList args = QStringList();
		args.append(QString::number(m_selectedKosmos->getId()));
		args.append(QString::number(m_member->getId()));
		args.append(QString::number(m_selectedKosmos->getCredit() ? -1 * m_selectedKosmos->getValue() : m_selectedKosmos->getValue() ));

		m_networkAdapter.reset(new NetworkAdapter(DELETE_KOSMOS_OPERATION,DELETE_REQ,args,this));
		int row = ui.tableView->selectionModel()->selectedRows()[0].row();
		ui.tableView->model()->removeRow(row);
		if (row > 0)
			ui.tableView->selectRow(row-1);
		else
			ui.tableView->selectRow(0);
	}
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void KosmosHistoric::extractKosmos(const QString&)
{
    m_kosmosExtraction.reset(new KosmosExtraction(m_member, this));
    m_kosmosExtraction->exec();
	m_kosmosExtraction.reset();
}
#pragma GCC diagnostic pop
