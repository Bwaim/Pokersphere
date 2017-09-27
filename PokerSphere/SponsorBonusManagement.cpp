#include "SponsorBonusManagement.h"

#include "requetes.h"
#include "Transform.h"

const char *SponsorBonusManagement::LAUNCHING[LAUNCHING_SIZE] = {"Déclenchement immédiat", "Déclenchement à la première adhésion"};
const char *SponsorBonusManagement::BONUS_TYPE[BONUS_TYPE_SIZE] = {"Gain Kosmos", "Gain en jour d'abonnement", "Gain Kosmos Bonus"};
const int SponsorBonusManagement::NB_MONTH[NB_MONTH_SIZE] = {0,1,3,6,12};

SponsorBonusManagement::SponsorBonusManagement(QWidget *parent)
    : QDialog(parent), m_selectedBonus(nullptr), m_bonus(QHash<int, std::shared_ptr<SponsorBonus>>())
{
    ui.setupUi(this);

    for (int i = 0; i < LAUNCHING_SIZE ; i++)
    {
        ui.typeDeclenchementComboBox->addItem(LAUNCHING[i]);
    }
	ui.typeDeclenchementComboBox->setCurrentIndex(-1);

    for (int i = 0; i < BONUS_TYPE_SIZE ; i++)
    {
        ui.typeGodsonGaincomboBox->addItem(BONUS_TYPE[i]);
        ui.typeSponsorGaincomboBox->addItem(BONUS_TYPE[i]);
    }
	ui.typeGodsonGaincomboBox->setCurrentIndex(0);
    ui.typeSponsorGaincomboBox->setCurrentIndex(0);

    for (int i = 0; i < NB_MONTH_SIZE ; i++)
    {
        ui.nbMonthMembershipComboBox->addItem(QString::number(NB_MONTH[i]));
    }
	ui.nbMonthMembershipComboBox->setCurrentIndex(0);

	
	connect(ui.typeDeclenchementComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(typeLaunchingChanged(int)));

	m_networkAdapter.reset(new NetworkAdapter(SELECT_SPONSOR_BONUS,SELECT,QStringList(),this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getBonus(QStringList, QList<QStringList>)));
	connect(ui.enregistrerModificationLabel,SIGNAL(linkActivated(QString)),this,SLOT(updateBonus(QString)));
}

SponsorBonusManagement::~SponsorBonusManagement()
{

}

void SponsorBonusManagement::getBonus(QStringList headers, QList<QStringList> result)
{
	int row = 0;
	if (m_selectedBonus != nullptr && ui.tableView->selectionModel()->selectedRows().size() > 0)
		row = ui.tableView->selectionModel()->selectedRows()[0].row();

	if (ui.tableView->selectionModel() != nullptr)
		delete ui.tableView->selectionModel();
	if (ui.tableView->model() != nullptr)
		delete ui.tableView->model();
	if (m_bonus.count() > 0)
	{
		QHash<int, std::shared_ptr<SponsorBonus>>::iterator i = m_bonus.begin();
		while (i != m_bonus.end())
		{
			i = m_bonus.erase(i);
		}
	}

	m_bonus = Transform::toSponsorBonus(headers,result);
	ui.tableView->setModel(Transform::toModelSponsorBonus(headers,result));
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
void SponsorBonusManagement::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
	int id = current.model()->data(current.sibling(current.row(),0)).toInt();
	displayBonus(m_bonus.value(id));
}
#pragma GCC diagnostic pop

void SponsorBonusManagement::displayBonus(std::shared_ptr<SponsorBonus> bonus)
{
    int offset = 0;
	ui.typeDeclenchementComboBox->setCurrentIndex(bonus->getLaunching());

    if (bonus->getLaunching() == 0)
    {
        if (bonus->getTypeGodsonGain() > 0)
            offset = 1;
    }

    ui.typeGodsonGaincomboBox->setCurrentIndex(bonus->getTypeGodsonGain() - offset);
    ui.godsonGainSpinBox->setValue(bonus->getGodsonGain());

    offset = 0;
    if (bonus->getLaunching() == 0)
    {
        if (bonus->getTypeSponsorGain() > 0)
            offset = 1;
    }

    ui.typeSponsorGaincomboBox->setCurrentIndex(bonus->getTypeSponsorGain() - offset);
    ui.sponsorGainSpinBox->setValue(bonus->getSponsorGain());

    ui.nbMonthMembershipComboBox->setCurrentIndex(ui.nbMonthMembershipComboBox->findText(QString::number(bonus->getNbMonthFirstMembership())));
	
	m_selectedBonus = bonus;
}

void SponsorBonusManagement::updateBonus(const QString&)
{
    int offset = 0;
	QStringList args = QStringList();
	ui.tableView->selectionModel()->disconnect(SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
	int rowNum = ui.tableView->selectionModel()->selectedRows()[0].row();
	QList<QStandardItem*> row = ((QStandardItemModel*)ui.tableView->model())->takeRow(rowNum);
	
    args.append(QString::number(m_selectedBonus->getId()));

	args.append(QString::number(ui.typeDeclenchementComboBox->currentIndex()));
	row[1]->setText(ui.typeDeclenchementComboBox->currentText());
	m_selectedBonus->setLaunching(ui.typeDeclenchementComboBox->currentIndex());

    if (ui.typeDeclenchementComboBox->currentIndex() == 0)
    {
        if (ui.typeGodsonGaincomboBox->currentIndex() > 0)
            offset = 1;
    }

    args.append(QString::number(ui.typeGodsonGaincomboBox->currentIndex() + offset));
	row[2]->setText(ui.typeGodsonGaincomboBox->currentText());
	m_selectedBonus->setTypeGodsonGain(ui.typeGodsonGaincomboBox->currentIndex() + offset);
	
	args.append(QString::number(ui.godsonGainSpinBox->value()));
	row[3]->setText(QString::number(ui.godsonGainSpinBox->value()));
	m_selectedBonus->setGodsonGain(ui.godsonGainSpinBox->value());

    offset = 0;
    if (ui.typeDeclenchementComboBox->currentIndex() == 0)
    {
        if (ui.typeSponsorGaincomboBox->currentIndex() > 0)
            offset = 1;
    }

    args.append(QString::number(ui.typeSponsorGaincomboBox->currentIndex() + offset));
	row[4]->setText(ui.typeSponsorGaincomboBox->currentText());
	m_selectedBonus->setTypeSponsorGain(ui.typeSponsorGaincomboBox->currentIndex() + offset);
	
	args.append(QString::number(ui.sponsorGainSpinBox->value()));
	row[5]->setText(QString::number(ui.sponsorGainSpinBox->value()));
	m_selectedBonus->setSponsorGain(ui.sponsorGainSpinBox->value());
	
    if (ui.nbMonthMembershipComboBox->currentIndex() == -1)
    {
        args.append(QString(""));
    }
    else
    {
        args.append(ui.nbMonthMembershipComboBox->currentText());
    }
	row[6]->setText(ui.nbMonthMembershipComboBox->currentText());
	m_selectedBonus->setNbMonthFirstMembership(ui.nbMonthMembershipComboBox->currentText().toInt());
	
	m_networkAdapter.reset(new NetworkAdapter(UPDATE_SPONSOR_BONUS,UPDATE,args,this));

	((QStandardItemModel*)ui.tableView->model())->insertRow(rowNum,row);
	connect(ui.tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
	ui.tableView->selectRow(rowNum);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void SponsorBonusManagement::typeLaunchingChanged(int index)
{
    if (ui.typeDeclenchementComboBox->currentIndex() == 0)
    {
        ui.nbMonthMembershipComboBox->setCurrentIndex(-1);
	    ui.nbMonthMembershipComboBox->setEnabled(false);

        ui.typeGodsonGaincomboBox->clear();
        ui.typeSponsorGaincomboBox->clear();
       
        for (int i = 0; i < BONUS_TYPE_SIZE ; i++)
        {
            if (i != 1)
            {
                ui.typeGodsonGaincomboBox->addItem(BONUS_TYPE[i]);
                ui.typeSponsorGaincomboBox->addItem(BONUS_TYPE[i]);
            }
        }
    }
    else
    {
        ui.nbMonthMembershipComboBox->setEnabled(true);

        ui.typeGodsonGaincomboBox->clear();
        ui.typeSponsorGaincomboBox->clear();
    
        for (int i = 0; i < BONUS_TYPE_SIZE ; i++)
        {
            ui.typeGodsonGaincomboBox->addItem(BONUS_TYPE[i]);
            ui.typeSponsorGaincomboBox->addItem(BONUS_TYPE[i]);
        }
    }
}
#pragma GCC diagnostic pop
