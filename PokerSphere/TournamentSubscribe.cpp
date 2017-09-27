#include "TournamentSubscribe.h"

#include "requetes.h"
#include "constantes.h"
#include "Transform.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>

TournamentSubscribe::TournamentSubscribe(std::shared_ptr<Tournament> tournament, QWidget *parent)
	: QDialog(parent), m_tournament(tournament), m_networkAdapter(nullptr), m_membership(nullptr), m_selectedMember(nullptr), m_networkAdapterPhoto(nullptr)
{
	ui.setupUi(this);

	QStringList args = QStringList();
	args.append(ui.filtreLineEdit->text());
    m_networkAdapter.reset(new NetworkAdapter(SELECT_MEMBERS_TO_SUBSCRIBE,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
	connect(ui.gererAdhesionLabel,SIGNAL(linkActivated(QString)),this,SLOT(showMembership(QString)));
	connect(ui.filtrerPushButton,SIGNAL(clicked(bool)),this,SLOT(doFilter(bool)));
	connect(ui.inscrireLabel,SIGNAL(linkActivated(QString)),this,SLOT(subscribe(QString)));
    connect(ui.tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClicked(QModelIndex)));
    connect(ui.filterFranchisePushButton,SIGNAL(clicked(bool)),this,SLOT(doFilterFranchise(bool)));

	resize(baseSize().width(),QApplication::desktop()->size().height()-75);
}

TournamentSubscribe::~TournamentSubscribe()
{
}

void TournamentSubscribe::getMembers(QStringList headers, QList<QStringList> result)
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
			//delete i.value();
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
void TournamentSubscribe::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
	int id = current.model()->data(current.sibling(current.row(),0)).toInt();
	m_selectedMember = m_members.value(id);

	QStringList args;
	args.append(QString::number(id));
	m_networkAdapterPhoto.reset(new NetworkAdapter(PHOTO,GET_FILE,args,this));
    connect(m_networkAdapterPhoto.get(), SIGNAL(finished(QStringList,QString)), this ,SLOT(updatePhoto(QStringList,QString)));
}
#pragma GCC diagnostic pop

void TournamentSubscribe::doubleClicked(const QModelIndex &current)
{
    int id = current.model()->data(current.sibling(current.row(),0)).toInt();
	m_selectedMember = m_members.value(id);
    subscribe(QString());
}

void TournamentSubscribe::showMembership(const QString&)
{
	m_membership.reset(new Membership(m_selectedMember, this));
	m_membership->exec();
	m_membership.reset();

	QStringList args = QStringList();
	args.append(ui.filtreLineEdit->text());
    m_networkAdapter.reset(new NetworkAdapter(SELECT_MEMBERS_TO_SUBSCRIBE,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
}

void TournamentSubscribe::doFilter(bool)
{
	QStringList args = QStringList();
	args.append(ui.filtreLineEdit->text());
    m_networkAdapter.reset(new NetworkAdapter(SELECT_MEMBERS_TO_SUBSCRIBE,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
    ui.gererAdhesionLabel->show();
}

void TournamentSubscribe::doFilterFranchise(bool)
{
    if (ui.filtreLineEdit->text().length() < 4)
    {
        QMessageBox::critical(this, ERREUR, FILTER_FRANCHISE_SIZE);
        return;
    }

    QStringList args = QStringList();
    args.append(ui.filtreLineEdit->text());
    m_networkAdapter.reset(new NetworkAdapter(SELECT_MEMBERS_FRANCHISE_TO_SUBSCRIBE,SELECT,args,this));
    connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
    ui.gererAdhesionLabel->hide();
}

void TournamentSubscribe::subscribe(const QString&)
{
	QStringList args = QStringList();
	args.append(QString::number(m_tournament->getId()));
	args.append(QString::number(m_selectedMember->getId()));
	int retard = ui.retardCheckBox->checkState() == Qt::Checked ? 1 : 0;
	args.append(QString::number(retard));
	m_networkAdapter.reset(new NetworkAdapter(CREATE_TOURNAMENT_SUBSCRIPTION,INSERT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
}

void TournamentSubscribe::updatePhoto(QStringList infos, QString data)
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
