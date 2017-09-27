#include "retraitkosmos.h"
#include "constantes.h"
#include "requetes.h"
#include "Transform.h"

RetraitKosmos::RetraitKosmos(std::shared_ptr<Member> member, QWidget *parent)
	: QDialog(parent), m_member(member), m_networkAdapter(nullptr)
{
	ui.setupUi(this);

	for (int i = 0 ; i < TAILLE_TYPE_PARTIE ; i++)
		ui.typePartieComboBox->addItem(QString(TYPE_PARTIE[i]));
	ui.typePartieComboBox->setCurrentIndex(2);

	ui.identifiantLabel->setText(QString("%1 %2").arg(m_member->getFirstname(),m_member->getName()));
	ui.capitalKosmosPtsLabel->setText(QString("%L1").arg(m_member->getKosmos()));
	ui.kosmosDisponiblesPtsLabel->setText(QString("%L1").arg(m_member->getKosmosDisponible()));

	ui.kosmosHorizontalSlider->setMaximum(m_member->getKosmosDisponible());
	ui.kosmosHorizontalSlider->setTickInterval(m_member->getKosmosDisponible() / 10);

	ui.kosmosSpinBox->setMaximum(m_member->getKosmosDisponible());

	QStringList args;
	args.append(QString::number(m_member->getId()));
	m_networkAdapter.reset(new NetworkAdapter(PHOTO,GET_FILE,args,this));
    connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QString)), this ,SLOT(updatePhoto(QStringList,QString)));

	connect(ui.kosmosHorizontalSlider, SIGNAL(valueChanged(int)), ui.kosmosSpinBox, SLOT(setValue(int)));
	connect(ui.kosmosSpinBox, SIGNAL(valueChanged(int)), ui.kosmosHorizontalSlider, SLOT(setValue(int)));
	connect(ui.kosmosSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));
	connect(ui.annulerRetraitLabel,SIGNAL(linkActivated(QString)),this,SLOT(cancelRetrait(QString)));
	connect(ui.effectuerRetraitLabel,SIGNAL(linkActivated(QString)),this,SLOT(saveRetrait(QString)));
}

RetraitKosmos::~RetraitKosmos()
{
}

void RetraitKosmos::setValue(int value)
{
	ui.capitalKosmosPtsLabel->setText(QString("%L1").arg(m_member->getKosmos() - value));
	ui.kosmosDisponiblesPtsLabel->setText(QString("%L1").arg(m_member->getKosmosDisponible() - value));
	ui.kosmosDemandesPtsLabel->setText(QString("%L1").arg(-value));
}

void RetraitKosmos::cancelRetrait(const QString&)
{
	reject();
}

void RetraitKosmos::saveRetrait(const QString&)
{
	QStringList args = QStringList();
	args.append(QString::number(m_member->getId()));
	args.append(QString::number(ui.kosmosSpinBox->value()));
	args.append(QString::number(ui.typePartieComboBox->currentIndex()));
	
	m_networkAdapter.reset(new NetworkAdapter(CREATE_RETRAIT,INSERT,args,this));

	connect(m_networkAdapter.get(), SIGNAL(finished()), this ,SLOT(accept()));
}

void RetraitKosmos::updatePhoto(QStringList infos, QString data)
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
