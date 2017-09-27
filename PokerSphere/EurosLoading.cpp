#include "EurosLoading.h"
#include "ui_EurosLoading.h"
#include "requetes.h"
#include "Transform.h"

EurosLoading::EurosLoading(std::shared_ptr<Member> member, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EurosLoading),
    m_member(member), m_networkAdapter(nullptr)
{
    ui->setupUi(this);

    ui->identifiantLabel->setText(QString("%1 %2").arg(m_member->getFirstname(),m_member->getName()));

    QStringList args;
    args.append(QString::number(m_member->getId()));
    m_networkAdapter.reset(new NetworkAdapter(PHOTO,GET_FILE,args,this));
    connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QString)), this ,SLOT(updatePhoto(QStringList,QString)));

    connect(ui->annulerDepotLabel,SIGNAL(linkActivated(QString)),this,SLOT(cancelDepot(QString)));
    connect(ui->effectuerDepotLabel,SIGNAL(linkActivated(QString)),this,SLOT(saveDepot(QString)));
}

EurosLoading::~EurosLoading()
{
    delete ui;
}

void EurosLoading::cancelDepot(const QString&)
{
    reject();
}

void EurosLoading::saveDepot(const QString&)
{
    QStringList args = QStringList();
    args.append(QString::number(m_member->getId()));
    args.append(QString::number(ui->eurosSpinBox->value()));

    m_networkAdapter.reset(new NetworkAdapter(CREATE_EUROS_LOADING,INSERT,args,this));

    connect(m_networkAdapter.get(), SIGNAL(finished()), this ,SLOT(accept()));
}

void EurosLoading::updatePhoto(QStringList infos, QString data)
{
    QString infosStr = infos.join(QString(" - "));
    if (!data.contains(QString(NO_PHOTO)))
    {
        QByteArray ba(data.toStdString().c_str());
        ba = QByteArray::fromHex(ba);
        QPixmap pixmap;
        pixmap.loadFromData(ba);
        float ratioHeight = pixmap.height() / ui->imageLabel->height();
        Transform::addInfosToPhoto(&pixmap,m_member->getFranchise(), ratioHeight, infosStr);
        ui->imageLabel->setPixmap(pixmap);
    }
    else
    {
        QPixmap pixmap(320,240);
        pixmap.fill(Qt::black);
        float ratioHeight = pixmap.height() / ui->imageLabel->height();
        Transform::addInfosToPhoto(&pixmap,m_member->getFranchise(), ratioHeight, infosStr);
        ui->imageLabel->setPixmap(pixmap);
    }
}
