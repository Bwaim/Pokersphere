#include "MemberElimination.h"

#include "requetes.h"
#include "DotationManager.h"

#include <QMessageBox>
#include <QHBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QLayoutItem>

MemberElimination::MemberElimination(std::shared_ptr<Tournament> tournament, std::shared_ptr<Member> member, std::shared_ptr<QMap<int, bool>> positions, QWidget *parent)
	: QDialog(parent), m_tournament(tournament), m_member(member), m_positions(positions), m_networkAdapter(nullptr)
    , m_dotationsWidgets(QList<QWidget*>())
{
	ui.setupUi(this);

	ui.identifiantLabel->setText(QString("%1 %2").arg(m_member->getFirstname(),m_member->getName()));

    if (m_tournament->getAttachedTournament() <= 0)
    {
        ui.inscrireTournoiLabel->setVisible(false);
        ui.inscrireTournoiCheckBox->setVisible(false);
    }

    ui.deleteDotationLabel->setVisible(false);

    connect(ui.addDotationLabel,SIGNAL(linkActivated(QString)),this,SLOT(addDotation(QString)));
    connect(ui.deleteDotationLabel,SIGNAL(linkActivated(QString)),this,SLOT(deleteDotation(QString)));
	connect(ui.annulerSortieLabel,SIGNAL(linkActivated(QString)),this,SLOT(cancelSortie(QString)));
	connect(ui.sortirLabel,SIGNAL(linkActivated(QString)),this,SLOT(saveSortie(QString)));
}

MemberElimination::~MemberElimination()
{
}

void MemberElimination::cancelSortie(const QString&)
{
	reject();
}

void MemberElimination::saveSortie(const QString&)
{
    if (positionIsValide() || (m_tournament->getAttachedTournament() > -1 && ui.inscrireTournoiCheckBox->isChecked()))
    {
        if (m_tournament->getAttachedTournament() > -1 && ui.inscrireTournoiCheckBox->isChecked())
        {
            QStringList args = QStringList();
	        args.append(QString::number(m_tournament->getAttachedTournament()));
	        args.append(QString::number(m_member->getId()));

            QEventLoop loop;
            m_networkAdapter.reset(new NetworkAdapter(CREATE_ATTACHED_TOURNAMENT_SUBSCRIPTION,INSERT,args,this));
            connect(m_networkAdapter.get(), SIGNAL(finished()), &loop ,SLOT(quit()));
            loop.exec();
	        
        }

	    QStringList args = QStringList();
	    args.append(QString::number(m_tournament->getId()));
	    args.append(QString::number(m_member->getId()));
        if (ui.positionLineEdit->text().compare("") == 0)
            args.append("2");
        else
	        args.append(ui.positionLineEdit->text());

        args.append(QString::number(ui.fidPointsSpinBox->value()));

        args.append(QString::number(ui.kosmosSpinBox->value()));

        int i = 0;
        while (i < m_dotationsWidgets.size())
        {
            args.append(static_cast<QComboBox*>(m_dotationsWidgets.at(i++))->currentText());
            args.append(QString::number(static_cast<QSpinBox*>(m_dotationsWidgets.at(i++))->value()));
            args.append(static_cast<QComboBox*>(m_dotationsWidgets.at(i++))->currentText());
        }


	    if (ui.kosmosSpinBox->value() > 0)
            m_networkAdapter.reset(new NetworkAdapter(UPDATE_TOURNAMENT_OUT_WITH_GAIN,UPDATE,args,this));
	    else
		    m_networkAdapter.reset(new NetworkAdapter(UPDATE_TOURNAMENT_OUT_WITHOUT_GAIN,UPDATE,args,this));

        connect(m_networkAdapter.get(), SIGNAL(finished()), this ,SLOT(accept()));
    }
    else
        QMessageBox::warning(this,ELIMINATION,QString(ELIMINATION_PHRASE_3));
}

bool MemberElimination::positionIsValide()
{
    if (ui.positionLineEdit->text().compare("") == 0)
        return false;
    return m_positions->value(ui.positionLineEdit->text().toInt());
}

void MemberElimination::addDotation(const QString&)
{
    DotationManager *dotationManager  = DotationManager::getInstance();

    QHBoxLayout *hlayout = new QHBoxLayout();
    ui.verticalDotationLayout->addLayout(hlayout);

    QComboBox *typeDotation = new QComboBox();
    dotationManager->fillTypes(typeDotation);
    hlayout->addWidget(typeDotation);

    QSpinBox *spinBox = new QSpinBox();
    hlayout->addWidget(spinBox);

    QComboBox *descriptionDotation = new QComboBox();
    dotationManager->fillDescriptions(descriptionDotation);
    hlayout->addWidget(descriptionDotation);

    m_dotationsWidgets.append(typeDotation);
    m_dotationsWidgets.append(spinBox);
    m_dotationsWidgets.append(descriptionDotation);

    ui.deleteDotationLabel->setVisible(true);

}

void MemberElimination::deleteDotation(const QString&)
{
    QLayoutItem *child;

    if ((child = ui.verticalDotationLayout->takeAt(ui.verticalDotationLayout->count()-1)) !=0 )
    {
        delete child;
        delete m_dotationsWidgets.takeLast(); // Description comboBox
        delete m_dotationsWidgets.takeLast(); // SpinBox
        delete m_dotationsWidgets.takeLast(); // Type comboBox
    }

    if (ui.verticalDotationLayout->count() == 0)
        ui.deleteDotationLabel->setVisible(false);
}
