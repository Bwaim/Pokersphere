#include "kosmosmanagement.h"
#include "requetes.h"
#include "Transform.h"
#include <QMessageBox>

KosmosManagement::KosmosManagement(QWidget *parent)
	: QWidget(parent), m_selectedMember(nullptr), m_member(QHash<int, std::shared_ptr<Member>>()), m_networkAdapterPhoto(nullptr)
{
	ui.setupUi(this);

	m_networkAdapter.reset(new NetworkAdapter(SELECT_ALL_MEMBERS_KOSMOS,SELECT,QStringList(),this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
	connect(ui.retraitKosmosLabel,SIGNAL(linkActivated(QString)),this,SLOT(showRetraitKosmos(QString)));
	connect(ui.depotKosmosLabel,SIGNAL(linkActivated(QString)),this,SLOT(showDepotKosmos(QString)));
	connect(ui.realiserEnchereLabel,SIGNAL(linkActivated(QString)),this,SLOT(showDoEnchere(QString)));
	connect(ui.historiqueLabel,SIGNAL(linkActivated(QString)),this,SLOT(showKosmosHistoric(QString)));
    connect(ui.tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClicked(QModelIndex)));
}

KosmosManagement::~KosmosManagement()
{
}

void KosmosManagement::getMembers(QStringList headers, QList<QStringList> result)
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
			//delete i.value();
			i = m_member.erase(i);
		}
	}

	m_member = Transform::toMembersKosmos(headers,result);
	ui.tableView->setModel(Transform::toModelKosmos(headers,result));
	ui.tableView->setColumnHidden(0,true);
	ui.tableView->setColumnHidden(1,true); // FRANCHISE
    ui.tableView->setColumnHidden(headers.size()-2,true); // ECHEANCE
	ui.tableView->setColumnHidden(headers.size()-1,true); // RC
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
void KosmosManagement::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
	int id = current.model()->data(current.sibling(current.row(),0)).toInt();
	int abonnement = current.model()->data(current.sibling(current.row(),11)).toInt();
	displayMember(m_member.value(id), abonnement);

	QStringList args;
	args.append(QString::number(id));
	m_networkAdapterPhoto.reset(new NetworkAdapter(PHOTO,GET_FILE,args,this));
    connect(m_networkAdapterPhoto.get(), SIGNAL(finished(QStringList,QString)), this ,SLOT(updatePhoto(QStringList,QString)));
}
#pragma GCC diagnostic pop

void KosmosManagement::doubleClicked(const QModelIndex &current)
{
    int id = current.model()->data(current.sibling(current.row(),0)).toInt();
    m_selectedMember = m_member.value(id);
    showKosmosHistoric(QString());
}

void KosmosManagement::displayMember(std::shared_ptr<Member> m, int abonnement)
{
	ui.prenomLabel->setText(m->getFirstname());
	ui.nomLabel->setText(m->getName());
	ui.capitalKosmosLabel->setText(QString("Capital Kosmos : %L1").arg(m->getKosmos()));
	ui.kosmosDisponiblesLabel->setText(QString("Kosmos Disponibles : %L1").arg(m->getKosmosDisponible()));
	ui.capitalEncheresLabel->setText(QString("Capital Enchères : %L1").arg(m->getKosmosDisponibleEnchere()));

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

	m_selectedMember = m;
}

void KosmosManagement::showRetraitKosmos(const QString&)
{
	if (m_selectedMember->getKosmosDisponible() > 0)
	{
		m_retraitKosmos.reset(new RetraitKosmos(m_selectedMember, this));
		int ret = m_retraitKosmos->exec();
		m_retraitKosmos.reset();

		if (ret == QDialog::Accepted)
		{
			m_networkAdapter.reset(new NetworkAdapter(SELECT_ALL_MEMBERS_KOSMOS,SELECT,QStringList(),this));
			connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
		}
	}
	else
		QMessageBox::information(this,INFOS,QString(PLUS_KOSMOS_DISPO));
}

void KosmosManagement::showDepotKosmos(const QString&)
{
	m_depotKosmos.reset(new DepotKosmos(m_selectedMember, this));
	int ret = m_depotKosmos->exec();
	m_depotKosmos.reset();

	if (ret == QDialog::Accepted)
	{
		m_networkAdapter.reset(new NetworkAdapter(SELECT_ALL_MEMBERS_KOSMOS,SELECT,QStringList(),this));
		connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
	}
}

void KosmosManagement::showDoEnchere(const QString&)
{
	if (m_selectedMember->getKosmosDisponibleEnchere() > 0)
	{
		m_createEnchere.reset(new CreateEnchere(m_selectedMember, this));
		int ret = m_createEnchere->exec();
		m_createEnchere.reset();

		if (ret == QDialog::Accepted)
		{
			m_networkAdapter.reset(new NetworkAdapter(SELECT_ALL_MEMBERS_KOSMOS,SELECT,QStringList(),this));
			connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
		}
	}
	else
		QMessageBox::information(this,INFOS,QString(PLUS_KOSMOS_DISPO_ENCHERES));
}

void KosmosManagement::showKosmosHistoric(const QString&)
{
	m_kosmosHistoric.reset(new KosmosHistoric(m_selectedMember, this));
	m_kosmosHistoric->exec();
	m_kosmosHistoric.reset();

	m_networkAdapter.reset(new NetworkAdapter(SELECT_ALL_MEMBERS_KOSMOS,SELECT,QStringList(),this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
}

void KosmosManagement::updatePhoto(QStringList infos, QString data)
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
