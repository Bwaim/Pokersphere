#include "CreateTournamentTemplate.h"

#include "constantes.h"
#include "TournamentTemplate.h"
#include "requetes.h"
#include "Parameter.h"

#include <QMessageBox>

CreateTournamentTemplate::CreateTournamentTemplate(int franchise, QWidget *parent)
    : QDialog(parent)
{
    init(franchise);

    ui.buyInSpinBox->setValue(250);
	ui.nbJoueurSpinBox->setValue(50);
}

CreateTournamentTemplate::CreateTournamentTemplate(QString name, int buyIn, int nbPlayers, int variante, QString comment, int franchise, QWidget *parent)
    : QDialog(parent)
{
    init(franchise);
    ui.nomLineEdit->setText(name);
    ui.buyInSpinBox->setValue(buyIn);
	ui.nbJoueurSpinBox->setValue(nbPlayers);
    ui.varianteComboBox->setCurrentIndex(variante);
    ui.commentLineEdit->setText(comment);
}

void CreateTournamentTemplate::init(int franchise)
{
    ui.setupUi(this);

    m_tournamentTemplateManager = TournamentTemplateManager::getInstance(franchise);

    for (int i = 0 ; i < TAILLE_VARIANTE ; i++)
		ui.varianteComboBox->addItem(QString(TYPE_VARIANTE[i]));

    connect(ui.annulerCreationLabel,SIGNAL(linkActivated(QString)),this,SLOT(cancelCreation(QString)));
	connect(ui.enregistrerTournoiTemplateLabel,SIGNAL(linkActivated(QString)),this,SLOT(saveTournamentTemplate(QString)));
}

CreateTournamentTemplate::~CreateTournamentTemplate()
{

}

void CreateTournamentTemplate::cancelCreation(const QString&)
{
	reject();
}

void CreateTournamentTemplate::saveTournamentTemplate(const QString&)
{
    std::shared_ptr<TournamentTemplate> tournamentTemplate = std::make_shared<TournamentTemplate>(ui.nomLineEdit->text(),
        ui.buyInSpinBox->value(), ui.nbJoueurSpinBox->value(), ui.varianteComboBox->currentIndex(), ui.commentLineEdit->text());

    if (!m_tournamentTemplateManager->add(tournamentTemplate,this))
    {
        QMessageBox::warning(this,CREATION_TEMPLATE_TOURNOI,QString(NOM_TEMPLATE_EXISTANT));
    }
    else
        m_tournamentTemplateManager->setLastTemplateAdded(tournamentTemplate);
}
