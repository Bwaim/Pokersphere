#include "createkosmosoperation.h"
#include "constantes.h"
#include "requetes.h"

CreateKosmosOperation::CreateKosmosOperation(std::shared_ptr<Member> member, QWidget *parent)
	: QDialog(parent), m_member(member), m_networkAdapter(nullptr)
{
	ui.setupUi(this);

	ui.dateOperationDateTimeEdit->setDateTime(QDateTime::currentDateTime());

	for ( int i = 0 ; i < TAILLE_TYPE_PARTIE_COMPLET ; i++)
		ui.typeOperationComboBox->addItem(QString(TYPE_PARTIE_COMPLET[i]));

	ui.typeOperationComboBox->setCurrentIndex(2);

	connect(ui.annulerCreationLabel,SIGNAL(linkActivated(QString)),this,SLOT(cancelCreation(QString)));
	connect(ui.enregistrerOperationLabel,SIGNAL(linkActivated(QString)),this,SLOT(saveOperation(QString)));
}

CreateKosmosOperation::~CreateKosmosOperation()
{
}

void CreateKosmosOperation::cancelCreation(const QString&)
{
	reject();
}

void CreateKosmosOperation::saveOperation(const QString&)
{
	QStringList args = QStringList();
	args.append(QString::number(m_member->getId()));
	args.append(QString::number(ui.valeurSpinBox->value()));
	args.append(ui.dateOperationDateTimeEdit->dateTime().toString(QString("dd/MM/yyyy HH:mm")));
	args.append(ui.creditRadioButton->isChecked() ? QString(UN) : QString(ZERO));
	args.append(QString::number(ui.typeOperationComboBox->currentIndex()));
	args.append(ui.infosAdditionnellesLineEdit->text());
	
	m_networkAdapter.reset(new NetworkAdapter(CREATE_KOSMOS_OPERATION,INSERT,args,this));

	connect(m_networkAdapter.get(), SIGNAL(finished()), this ,SLOT(accept()));
}
