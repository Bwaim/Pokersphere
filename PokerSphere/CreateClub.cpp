#include "CreateClub.h"
#include "ui_CreateClub.h"

#include <QMessageBox>
#include "requetes.h"
#include "Parameter.h"

CreateClub::CreateClub(int franchise, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateClub)
{
    init(franchise);
}

CreateClub::CreateClub(QString club, int franchise, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateClub)
{
    init(franchise);
    ui->clubLineEdit->setText(club);
}

void CreateClub::init(int franchise)
{
    ui->setupUi(this);

    setWindowTitle(QString("Création d'un club de ") + Parameter::getInstance()->getFranchiseLabel(franchise));

    m_clubManager = ClubManager::getInstance(franchise);

    connect(ui->annulerCreationLabel,SIGNAL(linkActivated(QString)),this,SLOT(cancelCreation(QString)));
    connect(ui->enregistrerClubLabel,SIGNAL(linkActivated(QString)),this,SLOT(saveClub(QString)));
}

CreateClub::~CreateClub()
{
    delete ui;
}

void CreateClub::cancelCreation(const QString&)
{
    reject();
}

void CreateClub::saveClub(const QString&)
{
    QString club = ui->clubLineEdit->text();

    if (!m_clubManager->add(club,this))
    {
        QMessageBox::warning(this,CREATION_CLUB,QString(NOM_CLUB_EXISTANT));
    }
}
