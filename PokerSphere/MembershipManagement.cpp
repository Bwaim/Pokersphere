#include "MembershipManagement.h"
#include "requetes.h"
#include "Transform.h"
#include "ChooseSponsor.h"
#include"Parameter.h"
#include "FranchiseSelection.h"
#include "ClubManager.h"

#include <QStandardItemModel>
#include <QStandardItem>
#include <QStringList>
#include <QList>
#include <QMessageBox>
#include <QStyledItemDelegate>

MembershipManagement::MembershipManagement(QWidget *parent)
	: QWidget(parent),m_createMembership(nullptr),m_membership(nullptr),m_selectedMember(nullptr), m_member(QHash<int, std::shared_ptr<Member>>()), m_photoChange(nullptr)
    , m_eurosLoading(nullptr), m_networkAdapterPhoto(nullptr), m_networkAdapterFranchise(nullptr), m_createClub(nullptr)
{
	ui.setupUi(this);

	m_networkAdapter.reset(new NetworkAdapter(SELECT_ALL_MEMBERS,SELECT,QStringList(),this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
	connect(ui.creerAdherentLabel,SIGNAL(linkActivated(QString)),this,SLOT(showCreateMembership(QString)));
	connect(ui.enregistrerModificationLabel,SIGNAL(linkActivated(QString)),this,SLOT(updateMembership(QString)));
	connect(ui.supprimerAdherentLabel,SIGNAL(linkActivated(QString)),this,SLOT(deleteMembership(QString)));
	connect(ui.gererAdhesionLabel,SIGNAL(linkActivated(QString)),this,SLOT(showMembership(QString)));
	connect(ui.changerImageLabel,SIGNAL(linkActivated(QString)),this,SLOT(showPhotoChange(QString)));
    connect(ui.tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClicked(QModelIndex)));
    connect(ui.sponsorToolButton,SIGNAL(clicked(bool)),this,SLOT(chooseSponsor(bool)));
    connect(ui.changerFranchiseLabel,SIGNAL(linkActivated(QString)),this,SLOT(changeFranchise(QString)));
    connect(ui.eurosLoadingLabel,SIGNAL(linkActivated(QString)),this,SLOT(showEurosLoading(QString)));
    connect(ui.addClubPushButton,SIGNAL(clicked(bool)),this,SLOT(showAddClub(bool)));

    Parameter *param = Parameter::getInstance();
    int franchise = param->getFranchise();

    if (franchise == 0)
    {
        connect(ui.filterPushButton, SIGNAL(clicked(bool)), this, SLOT(doFilter(bool)));
        m_networkAdapterFranchise.reset(new NetworkAdapter(SELECT_FRANCHISE,SELECT,QStringList(),this));
	    connect(m_networkAdapterFranchise.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(feedFranchise(QStringList, QList<QStringList>)));
    }
    else
    {
        ui.filterPushButton->setHidden(true);
        ui.franchiseComboBox->setHidden(true);
    }

    loadClubs(franchise);
}

MembershipManagement::~MembershipManagement()
{
}

void MembershipManagement::loadClubs(int franchise)
{
    ClubManager *clubManager = ClubManager::getInstance(franchise);
    clubManager->fillClubs(ui.clubComboBox);
}

void MembershipManagement::doFilter(bool)
{
	QStringList args = QStringList();
	
    int franchise = ui.franchiseComboBox->currentIndex();
    args.append(QString::number(franchise));
    
	m_networkAdapter.reset(new NetworkAdapter(SELECT_ALL_MEMBERS,SELECT,args,this));
    connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));

    loadClubs(franchise);
}

void MembershipManagement::getMembers(QStringList headers, QList<QStringList> result)
{
	int row = 0;
	if (m_selectedMember != nullptr && ui.tableView->selectionModel()->selectedRows().size() > 0)
		row = ui.tableView->selectionModel()->selectedRows()[0].row();
	
	if (ui.tableView->selectionModel() != nullptr)
		delete ui.tableView->selectionModel();
	if (ui.tableView->model() != nullptr)
		delete ui.tableView->model();
	if (m_member.count() > 0)
	{
		QHash<int, std::shared_ptr<Member>>::iterator i = m_member.begin();
		while (i != m_member.end())
		{
			i = m_member.erase(i);
		}
	}

	m_member = Transform::toMembers(headers,result);
	ui.tableView->setModel(Transform::toModelMembers(headers,result));
	ui.tableView->setColumnHidden(0,true);
    ui.tableView->setColumnHidden(headers.size()-7,true); //RC
    ui.tableView->setColumnHidden(headers.size()-3,true); //SPONSOR_ID
    ui.tableView->setColumnHidden(headers.size()-2,true); //FRANCHISE
	ui.tableView->resizeColumnsToContents();
	//ui.tableView->setItemDelegateForColumn(11,new QStyledItemDelegate());
	QItemSelectionModel *selectionModel = new QItemSelectionModel(ui.tableView->model());
	ui.tableView->setSelectionModel(selectionModel);
	connect(selectionModel, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));

	if (row < ui.tableView->model()->rowCount())
		ui.tableView->selectRow(row);
	else
		ui.tableView->selectRow(0);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void MembershipManagement::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
	int id = current.model()->data(current.sibling(current.row(),0)).toInt();
    int abonnement = current.model()->data(current.sibling(current.row(),15)).toInt();
	displayMember(m_member.value(id), abonnement);

	QStringList args;
	args.append(QString::number(id));
	m_networkAdapterPhoto.reset(new NetworkAdapter(PHOTO,GET_FILE,args,this));
    connect(m_networkAdapterPhoto.get(), SIGNAL(finished(QStringList,QString)), this ,SLOT(updatePhoto(QStringList,QString)));
}
#pragma GCC diagnostic pop

void MembershipManagement::doubleClicked(const QModelIndex &current)
{
    int id = current.model()->data(current.sibling(current.row(),0)).toInt();
    m_selectedMember = m_member.value(id);
    showMembership(QString());
}

void MembershipManagement::displayMember(std::shared_ptr<Member> m, int abonnement)
{
	ui.carteMagnetiqueLineEdit->setText(m->getCardNumber());
	ui.nomLineEdit->setText(m->getName());
	ui.prenomLineEdit->setText(m->getFirstname());
    ui.idUtilisateurLineEdit->setText(m->getLogin());
    ui.idPMULineEdit->setText(m->getIdPMU());
    ui.plaqueLineEdit->setText(m->getPlaqueImmatriculation());
	ui.naissanceDateEdit->setDate(m->getBirthDate());
	ui.adresseLineEdit->setText(m->getAdress());
	ui.codePostalLineEdit->setText(m->getZipCode());
	ui.villeLineEdit->setText(m->getCity());
	ui.portableLineEdit->setText(m->getPhone());
	ui.mailLineEdit->setText(m->getEmail());
	ui.kosmosSpinBox->setValue(m->getKosmos());
    ui.clubComboBox->setCurrentText(m->getClub());

	if (m->getEcheance().isNull())
		ui.echeanceLabel->setText(QString(ADHERENT_SANS_ABONNEMENT));
	else
		ui.echeanceLabel->setText(QString(ECHEANCE_PHRASE + m->getEcheance().toString(QString("dd/MM/yyyy"))));

	if (m->getEcheance() < QDate::currentDate())
		ui.colorLabel->setPixmap(QPixmap(":/img/Resources/rouge.gif"));
	else
		if (m->getEcheance() < QDate::currentDate().addDays(7))
			ui.colorLabel->setPixmap(QPixmap(":/img/Resources/jaune.gif"));
		else
			ui.colorLabel->setPixmap(QPixmap(":/img/Resources/vert.gif"));

	if (abonnement == 7)
		ui.onlineLabel->setPixmap(QPixmap(":/img/Resources/Earth.gif"));
	else
		ui.onlineLabel->setPixmap(QPixmap());

	if (m->getRc())
		ui.rcLabel->setText(QString(RC_PHRASE));
	else
		ui.rcLabel->setText(QString());

    ui.sponsorLineEdit->setText(m->getSponsor()->getName() + " " + m->getSponsor()->getFirstname());

	m_selectedMember = m;
}

void MembershipManagement::showCreateMembership(const QString&)
{
	m_createMembership.reset(new CreateMembership(this));
	int ret = m_createMembership->exec();
	m_createMembership.reset();

	if (ret == QDialog::Accepted)
	{
		m_networkAdapter.reset(new NetworkAdapter(SELECT_ALL_MEMBERS,SELECT,QStringList(),this));
		connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
	}
}

void MembershipManagement::showMembership(const QString&)
{
	m_membership.reset(new Membership(m_selectedMember, this));
    int ret = m_membership->exec();
	m_membership.reset();

    if (ret == QDialog::Accepted)
    {
        m_networkAdapter.reset(new NetworkAdapter(SELECT_ALL_MEMBERS,SELECT,QStringList(),this));
        connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
    }
}

void MembershipManagement::updateMembership(const QString&)
{
	QStringList args = QStringList();
	ui.tableView->selectionModel()->disconnect(SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
	int rowNum = ui.tableView->selectionModel()->selectedRows()[0].row();
	QList<QStandardItem*> row = ((QStandardItemModel*)ui.tableView->model())->takeRow(rowNum);
	
	args.append(QString::number(m_selectedMember->getId()));

	args.append(ui.nomLineEdit->text());
	row[1]->setText(ui.nomLineEdit->text());
	m_selectedMember->setName(ui.nomLineEdit->text());
	
	args.append(ui.prenomLineEdit->text());
	row[2]->setText(ui.prenomLineEdit->text());
	m_selectedMember->setFirstname(ui.prenomLineEdit->text());
	
    args.append(ui.idPMULineEdit->text());
    row[4]->setText(ui.idPMULineEdit->text());
    m_selectedMember->setIdPMU(ui.idPMULineEdit->text());
	
	args.append(ui.naissanceDateEdit->date().toString(QString("dd/MM/yyyy")));
	//row[4]->setText(ui.naissanceDateEdit->date().toString(QString("dd/MM/yyyy")));
	row[5]->setData(QVariant(ui.naissanceDateEdit->date()),Qt::DisplayRole);
	m_selectedMember->setBirthDate(ui.naissanceDateEdit->date());
	
	args.append(ui.adresseLineEdit->text());
	row[6]->setText(ui.adresseLineEdit->text());
	m_selectedMember->setAdress(ui.adresseLineEdit->text());
	
	args.append(ui.codePostalLineEdit->text());
	row[7]->setText(ui.codePostalLineEdit->text());
	m_selectedMember->setZipCode(ui.codePostalLineEdit->text());
	
	args.append(ui.villeLineEdit->text());
	row[8]->setText(ui.villeLineEdit->text());
	m_selectedMember->setCity(ui.villeLineEdit->text());
	
	args.append(ui.portableLineEdit->text());
	row[9]->setText(ui.portableLineEdit->text());
	m_selectedMember->setPhone(ui.portableLineEdit->text());
	
	args.append(ui.mailLineEdit->text());
	row[10]->setText(ui.mailLineEdit->text());
	m_selectedMember->setEmail(ui.mailLineEdit->text());
	
	args.append(QString::number(ui.kosmosSpinBox->value()));
	//row[12]->setText(QString::number(ui.kosmosSpinBox->value()));
	row[13]->setData(QVariant(ui.kosmosSpinBox->value()),Qt::DisplayRole);
	m_selectedMember->setKosmos(ui.kosmosSpinBox->value());
	
    args.append(ui.clubComboBox->currentText());
    row[14]->setText(ui.clubComboBox->currentText());
    m_selectedMember->setClub(ui.clubComboBox->currentText());

    args.append(QString::number(m_selectedMember->getSponsor()->getId()));
    row[17]->setText(m_selectedMember->getSponsor()->getName());
    row[18]->setText(m_selectedMember->getSponsor()->getFirstname());

    args.append(ui.plaqueLineEdit->text());
    row[21]->setText(ui.plaqueLineEdit->text());
    m_selectedMember->setPlaqueImmatriculation(ui.plaqueLineEdit->text());

    if (ui.carteMagnetiqueLineEdit->text().compare(m_selectedMember->getCardNumber()) != 0)
    {
        args.append(ui.carteMagnetiqueLineEdit->text());
        if (!cardNumberIsUse(ui.carteMagnetiqueLineEdit->text()))
        {
	        row[16]->setText(ui.carteMagnetiqueLineEdit->text());
            m_selectedMember->setCardNumber(ui.carteMagnetiqueLineEdit->text());
        }
    }

    m_networkAdapter.reset(new NetworkAdapter(UPDATE_MEMBER,UPDATE,args,this));

	((QStandardItemModel*)ui.tableView->model())->insertRow(rowNum,row);
	connect(ui.tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
	ui.tableView->selectRow(rowNum);
}

void MembershipManagement::deleteMembership(const QString&)
{
	if (QMessageBox::question(this,SUPP_ADHERENT,QString(SUPP_ADHERENT_PHRASE) 
		+ m_selectedMember->getName() + QString(" ") + m_selectedMember->getFirstname() + QString(" ?")
		,QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
	{
		QStringList args = QStringList();
		args.append(QString::number(m_selectedMember->getId()));

		m_networkAdapter.reset(new NetworkAdapter(DELETE_MEMBER,DELETE_REQ,args,this));
		int row = ui.tableView->selectionModel()->selectedRows()[0].row();
		ui.tableView->model()->removeRow(row);
		if (row > 0)
			ui.tableView->selectRow(row-1);
		else
			ui.tableView->selectRow(0);
	}
}

void MembershipManagement::updatePhoto(QStringList infos, QString data)
{
    QString infosStr = infos.join(QString(" - "));
    if (!data.contains(QString(NO_PHOTO)))
	{
        QByteArray ba(data.toStdString().c_str());
		ba = QByteArray::fromHex(ba);
		QPixmap pixmap;
		pixmap.loadFromData(ba);
        float ratioHeight = pixmap.height() / ui.imageLabel->height();
        Transform::addInfosToPhoto(&pixmap,m_selectedMember->getFranchise(), ratioHeight, infosStr);
		ui.imageLabel->setPixmap(pixmap);
	}
	else
    {
        QPixmap pixmap(320,240);
        pixmap.fill(Qt::black);
        float ratioHeight = pixmap.height() / ui.imageLabel->height();
        Transform::addInfosToPhoto(&pixmap,m_selectedMember->getFranchise(), ratioHeight, infosStr);
		ui.imageLabel->setPixmap(pixmap);
		//ui.imageLabel->setPixmap(QPixmap());
    }
}

void MembershipManagement::showPhotoChange(const QString&)
{
	m_photoChange.reset(new PhotoChange(m_selectedMember, this));
	m_photoChange->exec();
	m_photoChange.reset();

	QStringList args;
	args.append(QString::number(m_selectedMember->getId()));
	m_networkAdapterPhoto.reset(new NetworkAdapter(PHOTO,GET_FILE,args,this));
    connect(m_networkAdapterPhoto.get(), SIGNAL(finished(QStringList,QString)), this ,SLOT(updatePhoto(QStringList,QString)));
}

bool MembershipManagement::cardNumberIsUse(QString cardNumber)
{
    if (cardNumber.isEmpty())
        return false;

    for (int i = 0; i < ui.tableView->model()->rowCount() ; i ++)
    {
        QString data = ui.tableView->model()->index(i,16).data().toString();
        if (data.compare(cardNumber) == 0)
            return true;
    }

    return false;
}

void MembershipManagement::chooseSponsor(bool)
{
    ChooseSponsor chooseSponsor(this);
	connect(&chooseSponsor,SIGNAL(choose()),this,SLOT(updateSponsor()));
    chooseSponsor.exec();
}

void MembershipManagement::updateSponsor()
{
    ChooseSponsor *sponsor = static_cast<ChooseSponsor*>(sender());
    ui.sponsorLineEdit->setText(sponsor->getSelection()->getName() + " " + sponsor->getSelection()->getFirstname());
    m_selectedMember->setSponsor(sponsor->getSelection());
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void MembershipManagement::feedFranchise(QStringList headers, QList<QStringList> result)
{
    ui.franchiseComboBox->addItem(QString("France"));
    for (int i =0; i < result.size(); i++)
        ui.franchiseComboBox->addItem(result[i][1]);
}
#pragma GCC diagnostic pop

void MembershipManagement::changeFranchise(const QString&)
{
    int franchise = -1;
    FranchiseSelection franchiseSelection(&franchise);
    franchiseSelection.exec();

    if (franchise != -1)
    {
        QStringList args = QStringList();
        args.append(QString::number(m_selectedMember->getId()));
        args.append(QString::number(franchise));
        m_networkAdapter.reset(new NetworkAdapter(UPDATE_FRANCHISE_MEMBER,UPDATE,args,this));

        QEventLoop loop;
        connect(m_networkAdapter.get(), SIGNAL(finished()), &loop ,SLOT(quit()));
        loop.exec();

        m_networkAdapter.reset(new NetworkAdapter(SELECT_ALL_MEMBERS,SELECT,QStringList(),this));
        connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
    }
}

void MembershipManagement::showEurosLoading(const QString&)
{
    if (m_selectedMember)
    {
        m_eurosLoading.reset(new EurosLoading(m_selectedMember,this));
        m_eurosLoading->exec();
        m_eurosLoading.reset();
    }
}

void MembershipManagement::showAddClub(bool)
{
    Parameter *param = Parameter::getInstance();
    int franchise = param->getFranchise();

    if (franchise == 0)
        franchise = ui.franchiseComboBox->currentIndex();
    if (franchise > 0)
    {
        m_createClub.reset(new CreateClub(franchise, this));
        m_createClub->exec();
        m_createClub.reset();

        loadClubs(franchise);
    }
    else
        QMessageBox::warning(this,INFOS_MEMBRES,QString(INFOS_AJOUT_CLUB_PHRASE));
}
