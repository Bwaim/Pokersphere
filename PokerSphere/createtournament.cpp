#include "createtournament.h"
#include "constantes.h"
#include "requetes.h"
#include "Parameter.h"
#include "FranchiseSelection.h"

CreateTournament::CreateTournament(int typeTournament, QWidget *parent)
	: QDialog(parent), m_networkAdapter(nullptr), m_typeTournament(typeTournament), m_franchise(0), m_lastAttachedTournament(-2)
{
	init();

	ui.buyInSpinBox->setValue(250);
	ui.nbJoueurSpinBox->setValue(50);
}

CreateTournament::CreateTournament(QString name, int buyIn, int nbPlayers, int variante, QString comment, bool privat, int typeTournament
    , QDateTime beginDate, QDateTime endDate, int lastAttachedTournament, QWidget *parent)
    : QDialog(parent), m_networkAdapter(nullptr), m_typeTournament(typeTournament), m_lastAttachedTournament(lastAttachedTournament)
{
    init();

    ui.nomLineEdit->setText(name);
    ui.buyInSpinBox->setValue(buyIn);
	ui.nbJoueurSpinBox->setValue(nbPlayers);
    ui.varianteComboBox->setCurrentIndex(variante);
    ui.commentLineEdit->setText(comment);
    ui.privateCheckBox->setChecked(privat);
    ui.dateDebutDateTimeEdit->setDateTime(beginDate);
	ui.dateFinInscriptionDateTimeEdit->setDateTime(endDate);
}

CreateTournament::~CreateTournament()
{
}

void CreateTournament::init()
{
    ui.setupUi(this);

	ui.dateDebutDateTimeEdit->setDateTime(QDateTime(QDate::currentDate(),QTime(19,0,0)));
	ui.dateFinInscriptionDateTimeEdit->setDateTime(QDateTime(QDate::currentDate(),QTime(19,0,0)));

	for (int i = 0 ; i < TAILLE_VARIANTE ; i++)
		ui.varianteComboBox->addItem(QString(TYPE_VARIANTE[i]));

    Parameter *param = Parameter::getInstance();
    m_franchise = param->getFranchise();
    if (m_franchise == 0)
    {
        FranchiseSelection franchiseSelection(&m_franchise);
        franchiseSelection.exec();
    }
    m_tournamentTemplateManager = TournamentTemplateManager::getInstance(m_franchise);
    ui.templateComboBox->addItem(QString(""));

    if (m_typeTournament == 1)
    {
        ui.creationTournoiLabel->setText(QString("Création d'un Sit & Go"));
        ui.enregistrerTournoiLabel->setText(QString("<a href=\"a\" style=\"text-decoration:none;\"><font color=\"#5E2F6A\">Enregistrer le Sit & Go</font></a>"));
    }

    QStringList args = QStringList();
	args.append(QString::number(m_franchise));
	m_networkAdapter.reset(new NetworkAdapter(SELECT_TOURNAMENT_ASSOCIATED,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(feedTournamentList(QStringList, QList<QStringList>)));

	connect(ui.dateDebutDateTimeEdit, SIGNAL(dateTimeChanged(QDateTime)), ui.dateFinInscriptionDateTimeEdit, SLOT(setDateTime(QDateTime)));
	connect(ui.annulerCreationLabel,SIGNAL(linkActivated(QString)),this,SLOT(cancelCreation(QString)));
	connect(ui.enregistrerTournoiLabel,SIGNAL(linkActivated(QString)),this,SLOT(saveTournament(QString)));
    connect(m_tournamentTemplateManager,SIGNAL(isLoaded(void)),this,SLOT(updateTemplateList(void)));

    updateTemplateList();
}

void CreateTournament::updateTemplateList()
{
    if (ui.templateComboBox->count() == 1)
    {
        QMap<QString, std::shared_ptr<TournamentTemplate>> templates = m_tournamentTemplateManager->getTemplates();
        QMap<QString, std::shared_ptr<TournamentTemplate>>::iterator i = templates.begin();
        while (i != templates.end())
	    {
            std::shared_ptr<TournamentTemplate> tournamentTemplate = i.value();
            ui.templateComboBox->addItem(tournamentTemplate->getName());
            i++;
        }

        connect(ui.templateComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateFromTemplate(QString)));
    }
}

void CreateTournament::cancelCreation(const QString&)
{
	reject();
}

void CreateTournament::saveTournament(const QString&)
{
	QStringList args = QStringList();
	args.append(ui.nomLineEdit->text());
	args.append(ui.dateDebutDateTimeEdit->dateTime().toString(QString("dd/MM/yyyy HH:mm")));
	args.append(ui.dateFinInscriptionDateTimeEdit->dateTime().toString(QString("dd/MM/yyyy HH:mm")));
	args.append(QString::number(ui.buyInSpinBox->value()));
	args.append(QString::number(ui.nbJoueurSpinBox->value()));
	args.append(QString::number(ui.varianteComboBox->currentIndex()));
	args.append(ui.commentLineEdit->text());
    args.append(QString::number(ui.privateCheckBox->checkState() == Qt::Unchecked ? 0 : 1));
    args.append(QString::number(m_typeTournament));

    if (m_franchise == 0)
    {
        FranchiseSelection franchiseSelection(&m_franchise);
        franchiseSelection.exec();
    }
    args.append(QString::number(m_franchise));
	args.append(QString::number(ui.tournoiLieComboBox->itemData(ui.tournoiLieComboBox->currentIndex()).toInt()));

	m_networkAdapter.reset(new NetworkAdapter(CREATE_TOURNAMENT,INSERT,args,this));

	connect(m_networkAdapter.get(), SIGNAL(finished()), this ,SLOT(accept()));
}

void CreateTournament::updateFromTemplate(const QString& templateName)
{
    if (!templateName.isEmpty())
    {
        std::shared_ptr<TournamentTemplate> tournamentTemplate = m_tournamentTemplateManager->get(templateName);

        ui.nomLineEdit->setText(tournamentTemplate->getName());
        ui.buyInSpinBox->setValue(tournamentTemplate->getBuyIn());
        ui.nbJoueurSpinBox->setValue(tournamentTemplate->getNbPlayers());
        ui.varianteComboBox->setCurrentIndex(tournamentTemplate->getVariante());
        ui.commentLineEdit->setText(tournamentTemplate->getComment());
    }
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void CreateTournament::feedTournamentList(QStringList headers, QList<QStringList> result)
{
    ui.tournoiLieComboBox->addItem("", -1);
    if (m_lastAttachedTournament == -1)
        ui.tournoiLieComboBox->setCurrentIndex(0);
    for (int i = 0 ; i < result.size() ; i++)
    {
        ui.tournoiLieComboBox->addItem(result[i][1], result[i][0].toInt());
        if (m_lastAttachedTournament == result[i][0].toInt())
            ui.tournoiLieComboBox->setCurrentIndex(i+1);
    }
}
#pragma GCC diagnostic pop
