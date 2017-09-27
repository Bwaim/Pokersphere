#include "createenchere.h"
#include "requetes.h"
#include "Transform.h"

CreateEnchere::CreateEnchere(std::shared_ptr<Member> member, QWidget *parent)
	: QDialog(parent), m_member(member), m_networkAdapter(nullptr)
{
	ui.setupUi(this);

	ui.identifiantLabel->setText(QString("%1 %2").arg(m_member->getFirstname(),m_member->getName()));
	ui.capitalKosmosPtsLabel->setText(QString("%L1").arg(m_member->getKosmos()));
	ui.kosmosDisponiblesPtsLabel->setText(QString("%L1").arg(m_member->getKosmosDisponibleEnchere()));

	ui.kosmosHorizontalSlider->setMaximum(m_member->getKosmosDisponibleEnchere());
	ui.kosmosHorizontalSlider->setTickInterval(m_member->getKosmosDisponibleEnchere() / 10);

	ui.kosmosSpinBox->setMaximum(m_member->getKosmosDisponibleEnchere());

	QStringList args;
	args.append(QString::number(m_member->getId()));
	m_networkAdapter.reset(new NetworkAdapter(PHOTO,GET_FILE,args,this));
    connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QString)), this ,SLOT(updatePhoto(QStringList,QString)));

	connect(ui.kosmosHorizontalSlider, SIGNAL(valueChanged(int)), ui.kosmosSpinBox, SLOT(setValue(int)));
	connect(ui.kosmosSpinBox, SIGNAL(valueChanged(int)), ui.kosmosHorizontalSlider, SLOT(setValue(int)));
	connect(ui.kosmosSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));
	connect(ui.annulerEnchereLabel,SIGNAL(linkActivated(QString)),this,SLOT(cancelEnchere(QString)));
	connect(ui.effectuerEnchereLabel,SIGNAL(linkActivated(QString)),this,SLOT(saveEnchere(QString)));
}

CreateEnchere::~CreateEnchere()
{
}

void CreateEnchere::setValue(int value)
{
	ui.capitalKosmosPtsLabel->setText(QString("%L1").arg(m_member->getKosmos() - value));
	ui.kosmosDisponiblesPtsLabel->setText(QString("%L1").arg(m_member->getKosmosDisponibleEnchere() - value));
	ui.montantEncherePtsLabel->setText(QString("%L1").arg(-value));
}

void CreateEnchere::cancelEnchere(const QString&)
{
	reject();
}

void CreateEnchere::saveEnchere(const QString&)
{
	QStringList args = QStringList();
	args.append(QString::number(m_member->getId()));
	args.append(QString::number(ui.kosmosSpinBox->value()));
	args.append(ui.commentLineEdit->text());
	
	m_networkAdapter.reset(new NetworkAdapter(CREATE_ENCHERE,INSERT,args,this));

	connect(m_networkAdapter.get(), SIGNAL(finished()), this ,SLOT(accept()));
}

void CreateEnchere::updatePhoto(QStringList infos, QString data)
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
