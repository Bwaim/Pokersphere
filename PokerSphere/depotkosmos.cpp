#include "depotkosmos.h"
#include "constantes.h"
#include "requetes.h"
#include "Transform.h"

DepotKosmos::DepotKosmos(std::shared_ptr<Member> member, QWidget *parent)
	: QDialog(parent), m_member(member), m_networkAdapter(nullptr)
{
	ui.setupUi(this);

	for (int i = 0 ; i < TAILLE_TYPE_PARTIE_DEPOT ; i++)
		ui.typePartieComboBox->addItem(QString(TYPE_PARTIE_DEPOT[i]));
	ui.typePartieComboBox->setCurrentIndex(2);

	ui.identifiantLabel->setText(QString("%1 %2").arg(m_member->getFirstname(),m_member->getName()));
	ui.capitalKosmosPtsLabel->setText(QString("%L1").arg(m_member->getKosmos()));

	QStringList args;
	args.append(QString::number(m_member->getId()));
	m_networkAdapter.reset(new NetworkAdapter(PHOTO,GET_FILE,args,this));
    connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QString)), this ,SLOT(updatePhoto(QStringList,QString)));

	connect(ui.kosmosSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));
	connect(ui.annulerDepotLabel,SIGNAL(linkActivated(QString)),this,SLOT(cancelDepot(QString)));
	connect(ui.effectuerDepotLabel,SIGNAL(linkActivated(QString)),this,SLOT(saveDepot(QString)));
}

DepotKosmos::~DepotKosmos()
{
}

void DepotKosmos::setValue(int value)
{
	ui.capitalKosmosPtsLabel->setText(QString("%L1").arg(m_member->getKosmos() + value));
	ui.kosmosDemandesPtsLabel->setText(QString("+ %L1").arg(value));
}

void DepotKosmos::cancelDepot(const QString&)
{
	reject();
}

void DepotKosmos::saveDepot(const QString&)
{
	QStringList args = QStringList();
	args.append(QString::number(m_member->getId()));
	args.append(QString::number(ui.kosmosSpinBox->value()));

	int index = ui.typePartieComboBox->currentIndex();
	if (index == 3)
	{
		index = 5;
	}
	args.append(QString::number(index));
	
	m_networkAdapter.reset(new NetworkAdapter(CREATE_DEPOT,INSERT,args,this));

	connect(m_networkAdapter.get(), SIGNAL(finished()), this ,SLOT(accept()));
}

void DepotKosmos::updatePhoto(QStringList infos, QString data)
{
    QString infosStr = infos.join(QString(" - "));
    if (!data.contains(QString(NO_PHOTO)))
	{
		QByteArray ba(data.toStdString().c_str());
		ba = QByteArray::fromHex(ba);
		QPixmap pixmap;
		pixmap.loadFromData(ba);
        float ratioHeight = pixmap.height() / ui.imageLabel->height();
        Transform::addInfosToPhoto(&pixmap,m_member->getFranchise(), ratioHeight, infosStr);
		ui.imageLabel->setPixmap(pixmap);
	}
	else
	{
        QPixmap pixmap(320,240);
        pixmap.fill(Qt::black);
        float ratioHeight = pixmap.height() / ui.imageLabel->height();
        Transform::addInfosToPhoto(&pixmap,m_member->getFranchise(), ratioHeight, infosStr);
		ui.imageLabel->setPixmap(pixmap);
		//ui.imageLabel->setPixmap(QPixmap());
    }
}
