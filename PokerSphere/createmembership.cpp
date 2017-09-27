#include "createmembership.h"
#include "requetes.h"
#include "constantes.h"
#include "Transform.h"
#include "Parameter.h"
#include "FranchiseSelection.h"
#include <QMessageBox>
#include <QBuffer>
#include <QToolButton>
#include <QHBoxLayout>

CreateMembership::CreateMembership(QWidget *parent)
    : QDialog(parent), m_networkAdapter(nullptr), m_camera(nullptr), m_timer(this), m_isCaptured(false), m_chooseSponsor(nullptr)
    , m_sponsor(nullptr), m_createClub(nullptr)
{
	ui.setupUi(this);
    
    Parameter *param = Parameter::getInstance();
    int franchise = param->getFranchise();

    ClubManager *clubManager = ClubManager::getInstance(franchise);
    clubManager->fillClubs(ui.clubComboBox);

	m_membershipManager = MembershipManager::getInstance();
	ui.typeAdhesionComboBox->addItems(m_membershipManager->getDisplayList());
	ui.typeAdhesionComboBox->setCurrentIndex(0);

	connect(ui.annulerLabel,SIGNAL(linkActivated(QString)),this,SLOT(cancelCreation(QString)));
	connect(ui.enregistrerLabel,SIGNAL(linkActivated(QString)),this,SLOT(saveCreation(QString)));
	connect(ui.shootLabel,SIGNAL(linkActivated(QString)),this,SLOT(savePhoto(QString)));
    connect(ui.sponsorToolButton,SIGNAL(clicked(bool)),this,SLOT(chooseSponsor(bool)));
    connect(ui.addClubPushButton,SIGNAL(clicked(bool)),this,SLOT(showAddClub(bool)));

    m_camera = new cv::VideoCapture(CV_CAP_ANY);
    if (!m_camera->isOpened())
	{
		QMessageBox::warning(this,WEBCAM,PB_WEBCAM);
		m_camera = nullptr;
	}
	else
	{
		connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateCamera()));
		m_timer.start(15);
	}
}

CreateMembership::~CreateMembership()
{
	if (m_camera)
        m_camera->release();
}

void CreateMembership::cancelCreation(const QString&)
{
	reject();
}

void CreateMembership::saveCreation(const QString&)
{
    if (ui.nomLineEdit->text().isEmpty() || ui.prenomLineEdit->text().isEmpty()
            || ui.mailLineEdit->text().isEmpty())
    {
        QMessageBox::critical(this, ERREUR, CREATION_INFORMATIONS_MISSING);
        return;
    }

    QStringList args = QStringList();
    args.append(ui.nomLineEdit->text());
    args.append(ui.prenomLineEdit->text());
    args.append(ui.idPmuLineEdit->text());
    args.append(ui.naissanceDateEdit->date().toString(QString("dd/MM/yyyy")));
    args.append(ui.adresseLineEdit->text());
    args.append(ui.codePostalLineEdit->text());
    args.append(ui.villeLineEdit->text());
    args.append(ui.portableLineEdit->text());
    args.append(ui.mailLineEdit->text());
    args.append(QString::number(ui.kosmosSpinBox->value()));

    args.append(ui.clubComboBox->currentText());

    args.append(QString::number(m_membershipManager->getValue(ui.typeAdhesionComboBox->currentText())));
    args.append(m_membershipManager->getEndDate(ui.typeAdhesionComboBox->currentText(),QDate::currentDate()).toString(QString("dd/MM/yyyy")));
    args.append(ui.cardNumberLineEdit->text());
    if (m_sponsor)
        args.append(QString::number(m_sponsor->getId()));
    else
        args.append(QString::number(-1));

    Parameter *param = Parameter::getInstance();
    int franchise = param->getFranchise();
    if (franchise == 0)
    {
        FranchiseSelection franchiseSelection(&franchise);
        franchiseSelection.exec();
    }
    args.append(QString::number(franchise));

    args.append(QString::number(m_membershipManager->getNbMonth(ui.typeAdhesionComboBox->currentText())));

    args.append(ui.plaqueLineEdit->text());

	const QPixmap *pixmap = ui.photoLabel->pixmap();
	QByteArray ba;
	if (pixmap && m_isCaptured)
	{
		
		QBuffer buffer(&ba);
		buffer.open(QIODevice::WriteOnly);
		if (pixmap->save(&buffer,JPG))
		{
			ba = ba.toHex();
			args.append(QString(ba));
		}
	}

	m_networkAdapter.reset(new NetworkAdapter(CREATE_MEMBER,INSERT,args,this));

	connect(m_networkAdapter.get(), SIGNAL(finished()), this ,SLOT(accept()));
}

void CreateMembership::updateCamera()
{
    cv::Mat mat;
    *m_camera >> mat;
    QImage image = Transform::cvMatToQImage(mat).convertToFormat(QImage::Format_Indexed8,Qt::MonoOnly);
    ui.photoLabel->setPixmap(QPixmap::fromImage(Transform::coloredQImageToMonoQImage(image)));
}

void CreateMembership::savePhoto(const QString&)
{
	if (m_camera)
	{
		m_timer.stop();
		updateCamera();
        m_camera->release();
        ui.shootLabel->setText(QString("<a href=\"a\" style=\"text-decoration:none;\"><font color=\"#5E2F6A\">Capturer</font></a>"));
		disconnect(ui.shootLabel,SIGNAL(linkActivated(QString)),this,SLOT(savePhoto(QString)));
		connect(ui.shootLabel,SIGNAL(linkActivated(QString)),this,SLOT(startCapture(QString)));
		m_isCaptured = true;
	}
}

void CreateMembership::startCapture(const QString&)
{

    m_camera = new cv::VideoCapture(CV_CAP_ANY);
    if (!m_camera->isOpened())
	{
		QMessageBox::warning(this,WEBCAM,PB_WEBCAM);
		m_camera = nullptr;
	}
	else
	{
		connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateCamera()));
		m_timer.start(15);
		ui.shootLabel->setText(QString("<a href=\"a\" style=\"text-decoration:none;\"><font color=\"#5E2F6A\">Shoot Him !</font></a>"));
		disconnect(ui.shootLabel,SIGNAL(linkActivated(QString)),this,SLOT(startCapture(QString)));
		connect(ui.shootLabel,SIGNAL(linkActivated(QString)),this,SLOT(savePhoto(QString)));
		m_isCaptured = false;
	}
}

void CreateMembership::chooseSponsor(bool)
{
    m_chooseSponsor.reset(new ChooseSponsor(this));
	connect(m_chooseSponsor.get(),SIGNAL(choose()),this,SLOT(updateSponsor()));
    m_chooseSponsor->exec();
	m_chooseSponsor.reset();
}

void CreateMembership::updateSponsor()
{
    ChooseSponsor *sponsor = static_cast<ChooseSponsor*>(sender());
    ui.sponsorLineEdit->setText(sponsor->getSelection()->getName() + " " + sponsor->getSelection()->getFirstname());
    m_sponsor = sponsor->getSelection();
}

void CreateMembership::showAddClub(bool)
{
    Parameter *param = Parameter::getInstance();
    int franchise = param->getFranchise();
    if (franchise == 0)
    {
        FranchiseSelection franchiseSelection(&franchise);
        franchiseSelection.exec();
    }
    if (franchise > 0)
    {
        m_createClub.reset(new CreateClub(franchise, this));
        m_createClub->exec();
        m_createClub.reset();

        ClubManager *clubManager = ClubManager::getInstance(franchise);
        clubManager->fillClubs(ui.clubComboBox);
    }
    else
        QMessageBox::warning(this,INFOS_MEMBRES,QString(INFOS_AJOUT_CLUB_PHRASE));
}
