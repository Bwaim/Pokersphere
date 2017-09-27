#include "KosmosExtraction.h"
#include "constantes.h"
#include "requetes.h"
#include "Extraction.h"
#include <QCheckbox>
#include <QCalendarWidget>
#include <QFileDialog>

KosmosExtraction::KosmosExtraction(std::shared_ptr<Member> member, QWidget *parent)
    : QDialog(parent), m_member(member), m_networkAdapter(nullptr)
{
    ui.setupUi(this);

    QPalette palette = QPalette();
    palette.setColor(QPalette::WindowText,QColor(Qt::white));

    for (int i = 0; i < TAILLE_TYPE_PARTIE_COMPLET ; i++)
    {
        std::unique_ptr<QCheckBox> checkbox = std::unique_ptr<QCheckBox>(new QCheckBox(QString(TYPE_PARTIE_COMPLET[i]),this));
        checkbox->setFont(QFont(QString("Arial Narrow"), 15));
        checkbox->setPalette(palette);
        checkbox->setCheckState(Qt::Checked);
        ui.checkBoxVerticalLayout->addWidget(checkbox.release());
    }

    QStringList args = QStringList();
	args.append(QString::number(m_member->getId()));

	m_networkAdapter = std::unique_ptr<NetworkAdapter>(new NetworkAdapter(SELECT_EXTRACT_KOSMOS_DATE,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(initDate(QStringList, QList<QStringList>)));

    connect(ui.annulerLabel,SIGNAL(linkActivated(QString)),this,SLOT(cancelExtraction(QString)));
    connect(ui.extraireLabel,SIGNAL(linkActivated(QString)),this,SLOT(extract(QString)));
}

KosmosExtraction::~KosmosExtraction()
{
    
}

void KosmosExtraction::cancelExtraction(const QString&)
{
    reject();
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void KosmosExtraction::initDate(QStringList headers, QList<QStringList> result)
{
    QStringList list = result[0];

    QDate initMinDate = QDate::fromString(list[0],QString("dd/MM/yyyy"));
    QDate initMaxDate = QDate::fromString(list[1],QString("dd/MM/yyyy"));

    ui.filtreDebutDateEdit->setDateRange(initMinDate,initMaxDate);
    ui.filtreDebutDateEdit->setDate(initMinDate);
    
    std::unique_ptr<QCalendarWidget> calendarWidget = std::unique_ptr<QCalendarWidget>(new QCalendarWidget());
    calendarWidget->setSelectedDate(initMinDate);
    ui.filtreDebutDateEdit->setCalendarWidget(calendarWidget.release());

    ui.filtreFinDateEdit->setDateRange(initMinDate,initMaxDate);
    ui.filtreFinDateEdit->setDate(initMaxDate);

    std::unique_ptr<QCalendarWidget> calendarMawWidget = std::unique_ptr<QCalendarWidget>(new QCalendarWidget());
    calendarMawWidget->setSelectedDate(initMaxDate);
    ui.filtreDebutDateEdit->setCalendarWidget(calendarMawWidget.release());
}
#pragma GCC diagnostic pop

void KosmosExtraction::extract(const QString&)
{
    QStringList args = QStringList();

    args.append(QString::number(m_member->getId()));
    args.append(ui.filtreDebutDateEdit->date().toString(QString("dd/MM/yyyy")));
    args.append(ui.filtreFinDateEdit->date().addDays(1).toString(QString("dd/MM/yyyy")));

    std::vector<int> gameType = std::vector<int>();

    for (int i = 0; i < ui.checkBoxVerticalLayout->count(); i++)
    {
        QCheckBox *checkbox = static_cast<QCheckBox*>(ui.checkBoxVerticalLayout->itemAt(i)->widget());
        if (checkbox->checkState() == Qt::Checked)
        {
            gameType.push_back(i);
        }
    }

    QString listGameType("");
    std::vector<int>::iterator i = gameType.begin();
    if (i != gameType.end())
    {
        listGameType += QString::number(*i);
        ++i;
    }
    while (i != gameType.end())
    {
        listGameType += QString(",") + QString::number(*i);
        ++i;
    }
    args.append(listGameType);

    m_networkAdapter = std::unique_ptr<NetworkAdapter>(new NetworkAdapter(SELECT_EXTRACT_KOSMOS,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(exportKosmos(QStringList, QList<QStringList>)));
}

void KosmosExtraction::exportKosmos(QStringList headers, QList<QStringList> result)
{
    QString filename = QFileDialog::getSaveFileName(this,QString(ENREGISTRER_FICHIER),QString("kosmos_") 
        + m_member->getFirstname() + QString("_") + m_member->getName() + QString(".csv"),"*.csv");

	if (!filename.isEmpty())
	{
        Extraction::extractToCSV(filename,headers,result);
	}
}
