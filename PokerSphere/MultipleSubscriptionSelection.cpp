#include "MultipleSubscriptionSelection.h"

#include <QApplication>
#include <QDesktopWidget>

#include "requetes.h"
#include "Transform.h"
#include "Parameter.h"
#include "FranchiseSelection.h"

MultipleSubscriptionSelection::MultipleSubscriptionSelection(std::shared_ptr<Tournament> tournament, QString idMembers, QString email, QString name
    , QString firstname, QWidget *parent)
    : QDialog(parent), m_tournament(tournament), m_networkAdapter(nullptr), m_networkAdapterPhoto(nullptr), m_selectedMember(nullptr), m_email(email)
    , m_name(name), m_firstname(firstname)
{
    ui.setupUi(this);

    QStringList args = QStringList();
    args.append(idMembers);

	m_networkAdapter.reset(new NetworkAdapter(SELECT_MULTIPLE_MEMBERS_TO_SUBSRIBE,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
	connect(ui.inscrireLabel,SIGNAL(linkActivated(QString)),this,SLOT(subscribe(QString)));
    connect(ui.creationLabel,SIGNAL(linkActivated(QString)),this,SLOT(create(QString)));
    connect(ui.tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClicked(QModelIndex)));

	resize(baseSize().width(),QApplication::desktop()->size().height()-75);
}

MultipleSubscriptionSelection::~MultipleSubscriptionSelection()
{

}

void MultipleSubscriptionSelection::getMembers(QStringList headers, QList<QStringList> result)
{
	int row = 0;
	if (m_selectedMember != nullptr && ui.tableView->selectionModel()->selectedRows().size() > 0)
		row = ui.tableView->selectionModel()->selectedRows()[0].row();

	if (ui.tableView->selectionModel() != nullptr)
		delete ui.tableView->selectionModel();
	if (ui.tableView->model() != nullptr)
		delete ui.tableView->model();
	if (m_members.count() > 0)
	{
		QHash<int, std::shared_ptr<Member>>::iterator i = m_members.begin();
		while (i != m_members.end())
		{
			i = m_members.erase(i);
		}
	}

	m_members = Transform::toMembers(headers,result);
	ui.tableView->setModel(Transform::toModelAddSubscribeMember(headers,result));
	ui.tableView->setColumnHidden(0,true);
    ui.tableView->setColumnHidden(headers.size()-1,true); // FRANCHISE

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
void MultipleSubscriptionSelection::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
	int id = current.model()->data(current.sibling(current.row(),0)).toInt();
	m_selectedMember = m_members.value(id);

	QStringList args;
	args.append(QString::number(id));
	m_networkAdapterPhoto.reset(new NetworkAdapter(PHOTO,GET_FILE,args,this));
    connect(m_networkAdapterPhoto.get(), SIGNAL(finished(QStringList,QString)), this ,SLOT(updatePhoto(QStringList,QString)));
}
#pragma GCC diagnostic pop

void MultipleSubscriptionSelection::doubleClicked(const QModelIndex &current)
{
    int id = current.model()->data(current.sibling(current.row(),0)).toInt();
	m_selectedMember = m_members.value(id);
    subscribe(QString());
}

void MultipleSubscriptionSelection::create(const QString&)
{
	QStringList args = QStringList();
	args.append(m_name);
	args.append(m_firstname);
	args.append(m_email);
	
    Parameter *param = Parameter::getInstance();
    int franchise = param->getFranchise();
    if (franchise == 0)
    {
        FranchiseSelection franchiseSelection(&franchise);
        franchiseSelection.exec();
    }
    args.append(QString::number(franchise));
    args.append(QString::number(m_tournament->getId()));

	m_networkAdapter.reset(new NetworkAdapter(CREATE_MEMBER_BY_MULTIPLE_SUBSCRIPTION,INSERT,args,this));

	connect(m_networkAdapter.get(), SIGNAL(finished()), this ,SLOT(accept()));
}

void MultipleSubscriptionSelection::subscribe(const QString&)
{
	QStringList args = QStringList();
	args.append(QString::number(m_tournament->getId()));
	args.append(QString::number(m_selectedMember->getId()));
	int retard = 0;
	args.append(QString::number(retard));
	m_networkAdapter.reset(new NetworkAdapter(CREATE_TOURNAMENT_SUBSCRIPTION,INSERT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished()), this ,SLOT(accept()));
}

void MultipleSubscriptionSelection::updatePhoto(QStringList infos, QString data)
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
