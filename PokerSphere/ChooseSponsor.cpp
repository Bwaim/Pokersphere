#include "ChooseSponsor.h"

#include "requetes.h"
#include "Transform.h"
#include <QApplication>
#include <QDesktopWidget>

ChooseSponsor::ChooseSponsor(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    QStringList args = QStringList();
	args.append(ui.filtreLineEdit->text());
	m_networkAdapter.reset(new NetworkAdapter(SELECT_SPONSOR_MEMBER,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
	connect(ui.filtrerPushButton,SIGNAL(clicked(bool)),this,SLOT(doFilter(bool)));
	connect(ui.chooseLabel,SIGNAL(linkActivated(QString)),this,SLOT(choose(QString)));
    connect(ui.tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClicked(QModelIndex)));

	resize(baseSize().width(),QApplication::desktop()->size().height()-75);
}

ChooseSponsor::~ChooseSponsor()
{

}

void ChooseSponsor::getMembers(QStringList headers, QList<QStringList> result)
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
void ChooseSponsor::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
	int id = current.model()->data(current.sibling(current.row(),0)).toInt();
	m_selectedMember = m_members.value(id);

	QStringList args;
	args.append(QString::number(id));
	m_networkAdapterPhoto.reset(new NetworkAdapter(PHOTO,GET_FILE,args,this));
    connect(m_networkAdapterPhoto.get(), SIGNAL(finished(QStringList,QString)), this ,SLOT(updatePhoto(QStringList,QString)));
}
#pragma GCC diagnostic pop

void ChooseSponsor::doubleClicked(const QModelIndex &current)
{
    int id = current.model()->data(current.sibling(current.row(),0)).toInt();
	m_selectedMember = m_members.value(id);
    choose(QString());
}

void ChooseSponsor::doFilter(bool)
{
	QStringList args = QStringList();
	args.append(ui.filtreLineEdit->text());
	m_networkAdapter.reset(new NetworkAdapter(SELECT_SPONSOR_MEMBER,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
}

void ChooseSponsor::choose(const QString&)
{
    emit choose();
    accept();
}

void ChooseSponsor::updatePhoto(QStringList infos, QString data)
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

std::shared_ptr<Member> ChooseSponsor::getSelection()
{
    return m_selectedMember;
}
