#include "createadhesion.h"
#include "requetes.h"

CreateAdhesion::CreateAdhesion(std::shared_ptr<Member> member, QDate endDate, QWidget *parent)
	: QDialog(parent), m_member(member), m_endDate(endDate), m_networkAdapter(nullptr)
{
	ui.setupUi(this);

	m_membershipManager = MembershipManager::getInstance();
	ui.typeAbonnementComboBox->addItems(m_membershipManager->getDisplayList());
	ui.typeAbonnementComboBox->setCurrentIndex(1);

	connect(ui.grouperAdhesionCheckBox,SIGNAL(stateChanged(int)), this, SLOT(groupChanged(int)));
	connect(ui.typeAbonnementComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(typeAbonnementChanged(int)));
	connect(ui.debutAbonnementDateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(beginDateChanged(QDate)));

	ui.debutAbonnementDateEdit->setDate(m_endDate);
	ui.debutAbonnementDateEdit->setEnabled(false);

	connect(ui.annulerCreationLabel,SIGNAL(linkActivated(QString)),this,SLOT(cancelCreation(QString)));
	connect(ui.enregistrerAdhesionLabel,SIGNAL(linkActivated(QString)),this,SLOT(saveCreation(QString)));
}

CreateAdhesion::~CreateAdhesion()
{
}

void CreateAdhesion::groupChanged(int state)
{
	if (state == Qt::Checked)
	{
		ui.debutAbonnementDateEdit->setDate(m_endDate);
		ui.debutAbonnementDateEdit->setEnabled(false);
	}
	else
	{
		ui.debutAbonnementDateEdit->setDate(QDate::currentDate());
		ui.debutAbonnementDateEdit->setEnabled(true);
	}
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void CreateAdhesion::typeAbonnementChanged(int index)
{
	ui.finAbonnementDateEdit->setDate(m_membershipManager->getEndDate(ui.typeAbonnementComboBox->currentText()
		,ui.debutAbonnementDateEdit->date()));
}
#pragma GCC diagnostic pop

#pragma GCC diagnostic ignored "-Wunused-parameter"
void CreateAdhesion::beginDateChanged(const QDate &date)
{
	typeAbonnementChanged(ui.typeAbonnementComboBox->currentIndex());
}
#pragma GCC diagnostic pop

void CreateAdhesion::cancelCreation(const QString&)
{
	reject();
}

void CreateAdhesion::saveCreation(const QString&)
{
	QStringList args = QStringList();
	args.append(QString::number(m_member->getId()));
	args.append(QString::number(m_membershipManager->getValue(ui.typeAbonnementComboBox->currentText())));
	args.append(ui.debutAbonnementDateEdit->date().toString(QString("dd/MM/yyyy")));
	args.append(ui.finAbonnementDateEdit->date().toString(QString("dd/MM/yyyy")));
    args.append(QString::number(ui.prixDoubleSpinBox->value(),'f',2));
    args.append(QString::number(m_membershipManager->getNbMonth(ui.typeAbonnementComboBox->currentText())));
	
	m_networkAdapter.reset(new NetworkAdapter(CREATE_ADHESION,INSERT,args,this));
	
	connect(m_networkAdapter.get(), SIGNAL(finished()), this ,SLOT(accept()));
}
