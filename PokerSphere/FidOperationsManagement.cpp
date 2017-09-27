#include "FidOperationsManagement.h"
#include "ui_FidOperationsManagement.h"

#include "requetes.h"
#include "Transform.h"
#include <QMessageBox>

FidOperationsManagement::FidOperationsManagement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FidOperationsManagement),
    m_selectedMember(nullptr), m_member(QHash<int, std::shared_ptr<Member>>()),
    m_networkAdapterPhoto(nullptr), m_networkAdapterFid(nullptr)
{
    ui->setupUi(this);

    m_networkAdapter.reset(new NetworkAdapter(SELECT_ALL_MEMBERS_FID_OPERATIONS,SELECT,QStringList(),this));
    connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
    connect(ui->historiqueLabel,SIGNAL(linkActivated(QString)),this,SLOT(showFidOperationsHistoric(QString)));
    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClicked(QModelIndex)));
}

FidOperationsManagement::~FidOperationsManagement()
{
    delete ui;
}

void FidOperationsManagement::getMembers(QStringList headers, QList<QStringList> result)
{
    int row = 0;
    if (m_selectedMember != nullptr && ui->tableView->selectionModel()->selectedRows().size() > 0)
        row = ui->tableView->selectionModel()->selectedRows()[0].row();

    if (ui->tableView->selectionModel() != nullptr)
        delete ui->tableView->selectionModel();
    if (ui->tableView->model() != nullptr)
        delete ui->tableView->model();
    if (m_member.count() > 0)
    {
        QHash<int, std::shared_ptr<Member>>::iterator i = m_member.begin();
        while (i != m_member.end())
        {
            i = m_member.erase(i);
        }
    }

    m_member = Transform::toMembersFidOperations(headers,result);
    ui->tableView->setModel(Transform::toModelFidOperations(headers,result));
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(1,true); // FRANCHISE
    ui->tableView->setColumnHidden(headers.size()-2,true); // ECHEANCE
    ui->tableView->setColumnHidden(headers.size()-1,true); // RC
    ui->tableView->resizeColumnsToContents();
    QItemSelectionModel *selectionModel = new QItemSelectionModel(ui->tableView->model());
    ui->tableView->setSelectionModel(selectionModel);
    connect(selectionModel, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));

    if (row < ui->tableView->model()->rowCount())
        ui->tableView->selectRow(row);
    else
        ui->tableView->selectRow(0);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void FidOperationsManagement::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    int id = current.model()->data(current.sibling(current.row(),0)).toInt();
    int abonnement = current.model()->data(current.sibling(current.row(),6)).toInt();
    displayMember(m_member.value(id), abonnement);

    QStringList args;
    args.append(QString::number(id));
    m_networkAdapterPhoto.reset(new NetworkAdapter(PHOTO,GET_FILE,args,this));
    connect(m_networkAdapterPhoto.get(), SIGNAL(finished(QStringList,QString)), this ,SLOT(updatePhoto(QStringList,QString)));

    m_networkAdapterFid.reset(new NetworkAdapter(SELECT_MEMBER_FID_POINTS,SELECT,args,this));
    connect(m_networkAdapterFid.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(updateFidPointsMember(QStringList,QList<QStringList>)));
}
#pragma GCC diagnostic pop

void FidOperationsManagement::doubleClicked(const QModelIndex &current)
{
    int id = current.model()->data(current.sibling(current.row(),0)).toInt();
    m_selectedMember = m_member.value(id);
    showFidOperationsHistoric(QString());
}

void FidOperationsManagement::displayMember(std::shared_ptr<Member> m, int abonnement)
{
    ui->prenomLabel->setText(m->getFirstname());
    ui->nomLabel->setText(m->getName());

    if (m->getEcheance().isNull())
        ui->echeanceLabel->setText(QString(ADHERENT_SANS_ABONNEMENT));
    else
        ui->echeanceLabel->setText(QString(ECHEANCE_PHRASE + m->getEcheance().toString(QString("dd/MM/yyyy"))));

    if (m->getEcheance() < QDate::currentDate())
        ui->colorLabel->setPixmap(QPixmap(":/img/Resources/rouge.gif"));
    else
        if (m->getEcheance() < QDate::currentDate().addDays(7))
            ui->colorLabel->setPixmap(QPixmap(":/img/Resources/jaune.gif"));
        else
            ui->colorLabel->setPixmap(QPixmap(":/img/Resources/vert.gif"));

    if (abonnement == 7)
        ui->onlineLabel->setPixmap(QPixmap(":/img/Resources/Earth.gif"));
    else
        ui->onlineLabel->setPixmap(QPixmap());

    m_selectedMember = m;
}

void FidOperationsManagement::showFidOperationsHistoric(const QString&)
{
    m_fidHistoric.reset(new FidHistoric(m_selectedMember, this));
    int ret = m_fidHistoric->exec();
    m_fidHistoric.reset();

    if (ret == QDialog::Accepted)
    {
        m_networkAdapter.reset(new NetworkAdapter(SELECT_ALL_MEMBERS_FID_OPERATIONS,SELECT,QStringList(),this));
        connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
    }
}

void FidOperationsManagement::updatePhoto(QStringList infos, QString data)
{
    QString infosStr = infos.join(QString(" - "));
    if (!data.contains(QString(NO_PHOTO)))
    {
        QByteArray ba(data.toStdString().c_str());
        ba = QByteArray::fromHex(ba);
        QPixmap pixmap;
        pixmap.loadFromData(ba);
        float ratioHeight = pixmap.height() / ui->imageLabel->height();
        Transform::addInfosToPhoto(&pixmap,m_selectedMember->getFranchise(), ratioHeight, infosStr);
        ui->imageLabel->setPixmap(pixmap);
    }
    else
    {
        QPixmap pixmap(320,240);
        pixmap.fill(Qt::black);
        float ratioHeight = pixmap.height() / ui->imageLabel->height();
        Transform::addInfosToPhoto(&pixmap,m_selectedMember->getFranchise(), ratioHeight, infosStr);
        ui->imageLabel->setPixmap(pixmap);
    }
}

void FidOperationsManagement::updateFidPointsMember(QStringList headers, QList<QStringList> result)
{
    Transform::toMemberFidPoints(headers,result, m_selectedMember);

    ui->fidPointsLabel->setText(QString("Points de fidélité : %L1").arg(m_selectedMember->getFidPoints()));
    ui->fidStatutPointsLabel->setText(QString("Points de statut : %L1").arg(m_selectedMember->getFidStatutPoints()));
    ui->fidStatutLabel->setText(QString("Statut : %1").arg(m_selectedMember->getFidStatut()));
}
