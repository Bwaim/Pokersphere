#include "TournamentDetail.h"

#include "requetes.h"
#include "Transform.h"
#include "kosmoshistoric.h"
#include "tournamentmanagement.h"
#include "pokersphere.h"
#include "Extraction.h"
#include "sendRankingHero.h"
#include "Parameter.h"

#include <QPushButton>
#include <QSignalMapper>
#include <QMessageBox>
#include <QCheckBox>
#include <QList>
#include <QApplication>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QTextStream>
#include <QProgressBar>

TournamentDetail::TournamentDetail(std::shared_ptr<Tournament> tournament, QWidget *parent)
	: m_tournament(tournament), m_networkAdapter(nullptr), m_membership(nullptr), m_tournamentSubscribe(nullptr), m_selectedMember(nullptr)
	, m_presents(), m_networkAdapterPhoto(nullptr), m_hasOut(false), m_nbIn(0), m_membersOut(QList<int>()), m_allowPresent(false), m_positions(nullptr)
{
	ui.setupUi(this);

    ui.rankingHeroLabel->setVisible(false);

    Parameter *param = Parameter::getInstance();
    QString title = windowTitle() + " : " + param->getFranchiseLabel(m_tournament->getFranchise()) + " - " + m_tournament->getName() + " - " + m_tournament->getBeginDate().toString(QString("dd/MM/yyyy HH:mm"));
    setWindowTitle(title);

	if (m_tournament->getIsValidated())
    {
    	ui.decaverJoueursLabel->setText(QString("<a href='a' style='text-decoration:none;'><font color='#5E2F6A'>Recaver joueurs</font></a>"));
        m_positions = std::make_shared<QMap<int,bool>>();
    }
    else
        ui.controlerLabel->setVisible(false);

	updateList();
	connect(ui.gererAdhesionLabel,SIGNAL(linkActivated(QString)),this,SLOT(showMembership(QString)));
    connect(ui.gererKosmosLabel,SIGNAL(linkActivated(QString)),this,SLOT(showKosmosHistoric(QString)));
	connect(ui.inscrireMembreLabel,SIGNAL(linkActivated(QString)),this,SLOT(showTournamentSubscribe(QString)));
    connect(ui.extraireMailLabel, SIGNAL(linkActivated(QString)), this, SLOT(extractMails(QString)));
	connect(ui.decaverJoueursLabel,SIGNAL(linkActivated(QString)),this,SLOT(startTournament(QString)));
	connect(this,SIGNAL(setPresent(bool)), parent, SLOT(doFilter(bool)));
	connect(ui.classementLabel, SIGNAL(linkActivated(QString)), this, SLOT(extractRanking(QString)));
    connect(ui.tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClicked(QModelIndex)));
    connect(((PokerSphere*)(((TournamentManagement*)(parent))->getMainWindow()))->getLogWindow(),SIGNAL(tournamentInscription()),this,SLOT(updateList()));
    //connect(ui.rankingHeroLabel, SIGNAL(linkActivated(QString)), this, SLOT(sendRankingHero(QString)));
    connect(ui.importerInscriptionLabel, SIGNAL(linkActivated(QString)), this, SLOT(importSubscription(QString)));
    connect(ui.controlerLabel,SIGNAL(linkActivated(QString)),this,SLOT(showResultControl(QString)));
    connect(ui.finaliserLabel,SIGNAL(linkActivated(QString)),this,SLOT(finalizeTournament(QString)));

	resize(baseSize().width(),QApplication::desktop()->size().height()-75);
    setAttribute(Qt::WA_DeleteOnClose);
}

TournamentDetail::~TournamentDetail()
{
    emit setPresent(true);
}

void TournamentDetail::updateList()
{
    QStringList args = QStringList();
	args.append(QString::number(m_tournament->getId()));
	m_networkAdapter.reset(new NetworkAdapter(SELECT_SUBSCRIBES_MEMBERS,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
}

void TournamentDetail::getMembers(QStringList headers, QList<QStringList> result)
{
	int row = 0;
	m_nbIn = 0;
	if (m_selectedMember != nullptr && ui.tableView->selectionModel()->selectedRows().size() > 0)
		row = ui.tableView->selectionModel()->selectedRows()[0].row();

	if (ui.tableView->selectionModel() != nullptr)
		delete ui.tableView->selectionModel();
	if (ui.tableView->model() != nullptr)
	{
		QAbstractItemModel *model = ui.tableView->model();
		for (int i = 0 ; i < result.size() ; i++)
		{
			delete ui.tableView->indexWidget(model->index(i,headers.size()+1));
			delete ui.tableView->indexWidget(model->index(i,headers.size()));
			delete ui.tableView->indexWidget(model->index(i,headers.size()-2));
		}
		delete ui.tableView->model();
	}
	if (m_members.count() > 0)
	{
		QHash<int, std::shared_ptr<Member>>::iterator i = m_members.begin();
		while (i != m_members.end())
		{
			//delete i.value();
			i = m_members.erase(i);
		}
	}
	m_membersOut.clear();

	m_members = Transform::toMembers(headers,result);
	ui.tableView->setModel(Transform::toModelSubscribesMembers(headers,result));
	ui.tableView->setColumnHidden(0,true);
    ui.tableView->setColumnHidden(1,true); // FRANCHISE
    
	QSignalMapper *signalMapperButton = new QSignalMapper();
	QSignalMapper *signalMapperCheckBox = new QSignalMapper();
	QSignalMapper *signalMapperOutButton = new QSignalMapper();
	QAbstractItemModel *model = ui.tableView->model();
    if (m_positions)
        m_positions->clear();
	for (int i = 0 ; i < result.size() ; i++)
	{
        /* Initialisation de la map des positions */
        if (m_positions)
            m_positions->insert(i+1,true);

		/* Add present Checkbox */
		QCheckBox *checkBox = new QCheckBox();
		if (m_presents.contains(result[i][0].toInt()))
			checkBox->setCheckState(Qt::Checked);
		else
		{
			if (result[i][headers.size()-2].toInt() == 1)
			{
				m_presents.insert(result[i][0].toInt());
				if (m_presents.contains(result[i][0].toInt()))
					checkBox->setCheckState(Qt::Checked);
				else
					checkBox->setCheckState(Qt::Unchecked);
			}
			else
				checkBox->setCheckState(Qt::Unchecked);
		}
		ui.tableView->setIndexWidget(model->index(i,headers.size()-2),checkBox);

		///signalMapperCheckBox->setMapping(checkBox,model->data(model->index(i,0)).toInt()); //Utiliser text au lieu de int
        signalMapperCheckBox->setMapping(checkBox,i);
		connect(checkBox, SIGNAL(clicked()), signalMapperCheckBox, SLOT(map()));

		/* Add unsubscribe bouton */
		if ( !m_tournament->getIsValidated())
		{
			QPushButton *button = new QPushButton(QIcon(QPixmap(":/img/Resources/cancel.gif")),QString());
			button->setAutoFillBackground(true);
			button->setBackgroundRole(QPalette::Base);
			button->setStyleSheet("background-color: white;"
								  "border-style: outset;"
								  "border-color: white;");
			ui.tableView->setIndexWidget(model->index(i,headers.size()),button);

			signalMapperButton->setMapping(button,model->data(model->index(i,0)).toInt());
			connect(button, SIGNAL(clicked()), signalMapperButton, SLOT(map()));
		}
		else
		{
			/* add out button */
			if (result[i][7].isEmpty())
			{
			
				QPushButton *outButton = new QPushButton(QIcon(QPixmap(":/img/Resources/right_arrow_orange.gif")),QString());
				outButton->setAutoFillBackground(true);
				outButton->setBackgroundRole(QPalette::Base);
				outButton->setStyleSheet("background-color: white;"
									  "border-style: outset;"
									  "border-color: white;");
				ui.tableView->setIndexWidget(model->index(i,headers.size()),outButton);

				signalMapperOutButton->setMapping(outButton,model->data(model->index(i,0)).toInt());
				connect(outButton, SIGNAL(clicked()), signalMapperOutButton, SLOT(map()));
			}
            else /* add in button */
            {
                if (!m_tournament->getIsSentToRankingHero())
                {
                    QPushButton *inButton = new QPushButton(QIcon(QPixmap(":/img/Resources/left_arrow_green.gif")),QString());
		            inButton->setAutoFillBackground(true);
		            inButton->setBackgroundRole(QPalette::Base);
		            inButton->setStyleSheet("background-color: white;"
								            "border-style: outset;"
								            "border-color: white;");
		            ui.tableView->setIndexWidget(model->index(i,headers.size()),inButton);

		            signalMapperOutButton->setMapping(inButton,model->data(model->index(i,0)).toInt());
		            connect(inButton, SIGNAL(clicked()), signalMapperOutButton, SLOT(map()));
                }
            }

			/*gestion des sorties */
			if (result[i][7].isEmpty())
				m_nbIn = i+1;
			else
				m_membersOut.append(result[i][0].toInt());
		}
	}
	connect(signalMapperButton, SIGNAL(mapped(int)), this, SLOT (unsubscribe(int)));
	connect(signalMapperCheckBox, SIGNAL(mapped(int)), this, SLOT (setPresent(int)));
	connect(signalMapperOutButton, SIGNAL(mapped(int)), this, SLOT (showMemberElimination(int)));

	ui.nbPresentsLabel->setText(QString("Presents : ") + QString::number(m_presents.size()) + QString("/") 
		+ QString::number(result.size()));

	if ( m_tournament->getIsValidated())
	{
		/* Vérification s'il y a déjà des sortants */
		if (result.size() > 0 && !result[result.size()-1][7].isEmpty())
			m_hasOut = true;
		else
			m_hasOut = false;

		/* ajout du bouton pour réintégrer le dernier sortant */
		/*if (m_nbIn < result.size() && !m_tournament->getIsSentToRankingHero())
		{
			QPushButton *inButton = new QPushButton(QIcon(QPixmap(":/img/Resources/left_arrow_green.gif")),QString());
			inButton->setAutoFillBackground(true);
			inButton->setBackgroundRole(QPalette::Base);
			inButton->setStyleSheet("background-color: white;"
									"border-style: outset;"
									"border-color: white;");
			ui.tableView->setIndexWidget(model->index(m_nbIn,headers.size()),inButton);

			signalMapperOutButton->setMapping(inButton,model->data(model->index(m_nbIn,0)).toInt());
			connect(inButton, SIGNAL(clicked()), signalMapperOutButton, SLOT(map()));
		}*/

	}

	/* désactivation du bouton d'extraction du classement si le tournoi n'est pas fini. */
	if (m_tournament->getIsValidated() && m_nbIn == 0 && result.size() > 0)
    {
		ui.classementLabel->setVisible(true);
        /*if (!m_tournament->getIsSentToRankingHero())
            ui.rankingHeroLabel->setVisible(true);
        else
            ui.rankingHeroLabel->setVisible(false);*/

        if (m_nbIn == 0)
            ui.finaliserLabel->setVisible(false);
    }
	else
    {
		ui.classementLabel->setVisible(false);
        //ui.rankingHeroLabel->setVisible(false);
        ui.finaliserLabel->setVisible(false);
    }

	ui.tableView->resizeColumnsToContents();
	QItemSelectionModel *selectionModel = new QItemSelectionModel(ui.tableView->model());
	ui.tableView->setSelectionModel(selectionModel);
	connect(selectionModel, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
	
	if (row < ui.tableView->model()->rowCount())
		ui.tableView->selectRow(row);
	else
		ui.tableView->selectRow(0);

    m_allowPresent = true;

    /* Gestion de la map des positions disponibles */
    for (int i = 1; i<=result.size();i++)
    {
        
    }
    for (int i = 0; i<result.size();i++)
    {
        if (!result[i][7].isEmpty()) // Position
            m_positions->insert(result[i][7].toInt(), false);
    }
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void TournamentDetail::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
	int id = current.model()->data(current.sibling(current.row(),0)).toInt();
	m_selectedMember = m_members.value(id);

	QStringList args;
	args.append(QString::number(id));
	m_networkAdapterPhoto.reset(new NetworkAdapter(PHOTO,GET_FILE,args,this));
    connect(m_networkAdapterPhoto.get(), SIGNAL(finished(QStringList,QString)), this ,SLOT(updatePhoto(QStringList,QString)));

    if (Parameter::getInstance()->getFranchiseLabel(m_tournament->getFranchise()).compare(m_selectedMember->getFranchise()) != 0)
        ui.gererAdhesionLabel->hide();
    else
        ui.gererAdhesionLabel->show();
}
#pragma GCC diagnostic pop

void TournamentDetail::doubleClicked(const QModelIndex &current)
{
    int id = current.model()->data(current.sibling(current.row(),0)).toInt();
	m_selectedMember = m_members.value(id);
    showMembership(QString());
}

void TournamentDetail::unsubscribe(int id)
{
	if ( !m_tournament->getIsValidated())
	{
		std::shared_ptr<Member> member = m_members.value(id);
		if (QMessageBox::question(this,DESINSCRIRE,QString(DESINSCRIRE_PHRASE) + member->getFirstname() + QString(" ")
			+ member->getName() + QString(" ? ") 
			,QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
		{
            QStringList args = QStringList();
			args.append(QString::number(m_tournament->getId()));
			args.append(QString::number(member->getId()));

			m_networkAdapter.reset(new NetworkAdapter(DELETE_UNSUBSCRIBE,DELETE_REQ,args,this));
            connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
            m_allowPresent = false;
			QAbstractItemModel *model = ui.tableView->model();
			for (int i = 0 ; i < model->rowCount(); i++)
			{
				if (model->data(model->index(i,0)).toInt() == id)
				{
					disconnect(ui.tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));

					delete ui.tableView->indexWidget(model->index(i,ui.tableView->model()->columnCount()));
					delete ui.tableView->indexWidget(model->index(i,ui.tableView->model()->columnCount()-2));
					model->removeRow(i);
					
					connect(ui.tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this ,SLOT(selectionChanged(QModelIndex, QModelIndex)));
					
					break;
				}
			}

            if (m_presents.contains(id))
		        m_presents.remove(id);

			ui.nbPresentsLabel->setText(QString(PRESENTS) + QString::number(m_presents.size()) + QString("/") 
				+ QString::number(ui.tableView->model()->rowCount()));
		}
	}
	else
	{
		QMessageBox::warning(this,DESINSCRIRE,QString(DESINSCRIRE_PHRASE_2));
	}
}

void TournamentDetail::showMembership(const QString&)
{
	m_membership.reset(new Membership(m_selectedMember, this));
	m_membership->exec();
	m_membership.reset();

	updateList();
}

void TournamentDetail::showKosmosHistoric(const QString&)
{
    KosmosHistoric kosmosHistoric(m_selectedMember, this);
    kosmosHistoric.exec();
	
	updateList();
}

void TournamentDetail::showTournamentSubscribe(const QString&)
{
	if ( !m_tournament->getIsValidated())
	{
		m_tournamentSubscribe.reset(new TournamentSubscribe(m_tournament, this));
		m_tournamentSubscribe->exec();
		m_tournamentSubscribe.reset();

		updateList();
	}
	else
	{
		QMessageBox::warning(this,INSCRIRE,QString(INSCRIRE_PHRASE));
	}
}

void TournamentDetail::startTournament(const QString&)
{
	if (!m_tournament->getIsValidated())
	{
		QStringList args = QStringList();
		args.append(QString::number(m_tournament->getId()));
		m_networkAdapter.reset(new NetworkAdapter(CREATE_TOURNAMENT_VALIDATION,INSERT,args,this));
		m_tournament->setIsValidated(true);
		connect(m_networkAdapter.get(), SIGNAL(finished()), this ,SLOT(close()));
	}
	else
	{
		if (!m_hasOut)
		{
			QStringList args = QStringList();
			args.append(QString::number(m_tournament->getId()));
			m_networkAdapter.reset(new NetworkAdapter(CREATE_TOURNAMENT_CANCEL,INSERT,args,this));
			m_tournament->setIsValidated(true);
			connect(m_networkAdapter.get(), SIGNAL(finished()), this ,SLOT(close()));
		}
		else
		{
			QMessageBox::warning(this,ANNULER,QString(ANNULER_PHRASE));
		}
	}
}

void TournamentDetail::setPresent(int numLine)
{
    if (m_allowPresent)
    {
        QAbstractItemModel *model = ui.tableView->model();
        int id = model->data(model->index(numLine,0)).toInt();
        QCheckBox *checkBox = (QCheckBox*)ui.tableView->indexWidget(model->index(numLine,model->columnCount()-3));

        QStringList args = QStringList();
	    args.append(QString::number(m_tournament->getId()));
	    args.append(QString::number(id));
        args.append(QString::number(checkBox->checkState() == Qt::Checked ? 1 : 0));

	    m_networkAdapter.reset(new NetworkAdapter(UPDATE_TOURNAMENT_PRESENTS,UPDATE,args,this));
	    connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getTournaments(QStringList, QList<QStringList>)));

	    if (m_presents.contains(id))
		    m_presents.remove(id);
	    else
		    m_presents.insert(id);

	    ui.nbPresentsLabel->setText(QString(PRESENTS) + QString::number(m_presents.size()) + QString("/") 
		    + QString::number(ui.tableView->model()->rowCount()));
    }
    else
        QMessageBox::warning(this,INFOS,QString(SET_PRESENT_NON_DISPO));
}

void TournamentDetail::updatePhoto(QStringList infos, QString data)
{
    QString infosStr = infos.join(QString(" - "));
    if (!data.contains(QString(NO_PHOTO)))
	{
		QByteArray ba(data.toStdString().c_str());
		ba = QByteArray::fromHex(ba);
		QPixmap pixmap;
		pixmap.loadFromData(ba);
        float ratioHeight = pixmap.height() / ui.imageLabel->height();
        Transform::addInfosToPhoto(&pixmap,m_selectedMember->getFranchise(), ratioHeight, infosStr);
		ui.imageLabel->setPixmap(pixmap);
	}
	else
	{
        QPixmap pixmap(320,240);
        pixmap.fill(Qt::black);
        float ratioHeight = pixmap.height() / ui.imageLabel->height();
        Transform::addInfosToPhoto(&pixmap,m_selectedMember->getFranchise(), ratioHeight, infosStr);
		ui.imageLabel->setPixmap(pixmap);
		//ui.imageLabel->setPixmap(QPixmap());
    }
}

void TournamentDetail::showMemberElimination(int id)
{
	if ( m_tournament->getIsValidated())
	{
		if ( !m_membersOut.contains(id))
		{
            m_memberElimination.reset(new MemberElimination(m_tournament, m_members[id], m_positions, this));
			m_memberElimination->exec();
            //m_memberElimination.reset();

			updateList();
		}
		else
		{
			if (QMessageBox::question(this,ELIMINATION,QString(ELIMINATION_PHRASE) 
				,QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
			{
                if (m_tournament->getAttachedTournament() > -1)
                {
                    QStringList args = QStringList();
	                args.append(QString::number(m_tournament->getAttachedTournament()));
	                args.append(QString::number(id));

                    QEventLoop loop;
                    m_networkAdapter.reset(new NetworkAdapter(DELETE_UNSUBSCRIBE,DELETE_REQ,args,this));
                    connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), &loop ,SLOT(quit()));
                    loop.exec();
                }

				QStringList args = QStringList();
				args.append(QString::number(m_tournament->getId()));
				args.append(QString::number(id));
				m_networkAdapter.reset(new NetworkAdapter(UPDATE_TOURNAMENT_OUT_CANCEL,UPDATE,args,this));
				connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(getMembers(QStringList, QList<QStringList>)));
			}
		}
	}
	else
		QMessageBox::warning(this,ELIMINATION,QString(ELIMINATION_PHRASE_2));
}

void TournamentDetail::extractRanking(const QString)
{
	QStringList args = QStringList();
	args.append(QString::number(m_tournament->getId()));
	m_networkAdapter.reset(new NetworkAdapter(SELECT_EXTRACT_TOURNAMENT_RANKING,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(extractRanking(QStringList, QList<QStringList>)));
}

void TournamentDetail::extractRanking(QStringList headers, QList<QStringList> result)
{
	QString filename = QFileDialog::getSaveFileName(this,QString(ENREGISTRER_FICHIER),
		QString(m_tournament->getName()).replace(QString(" "),QString("_")) + QString("_") 
		+ m_tournament->getBeginDate().toString(QString("yyyyMMdd")) +QString(".csv"),"*.csv");

	if (!filename.isEmpty())
	{
		QFile file(filename);

		if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) 
		{
			QMessageBox::warning(this,EXPORT,QString(PROBLEME_CREATION_FICHIER));
			return;
		}

		writeExportCSV(file,headers,result);
	}
}

void TournamentDetail::writeExportCSV(QFile &file, QStringList headers, QList<QStringList> result)
{
	QTextStream flux(&file);

	for ( int i = 0 ; i < headers.size() ; i++)
	{
		if (i!= 0)
			flux << ";";
		flux << headers[i];
	}

	for ( int i = 0 ; i< result.size() ; i++)
	{
		QStringList line = result[i];

		flux << "\n";

		for ( int j = 0 ; j < line.size() ; j++)
		{
			if (j!= 0)
				flux << ";";
			flux << line[j];
		}
	}
	QMessageBox::information(this,EXPORT,QString(EXPORT_SUCCES));
}

void TournamentDetail::extractMails(QStringList headers, QList<QStringList> result)
{
    QString filename = QFileDialog::getSaveFileName(this,QString(ENREGISTRER_FICHIER),QString("mails_tournois_") 
		+ QString::number(m_tournament->getId()) +QString(".csv"),"*.csv");

	if (!filename.isEmpty())
	{
        Extraction::extractToCSV(filename,headers,result);
	}
}

void TournamentDetail::extractMails(const QString&)
{
    QStringList args = QStringList();
    args.append(QString::number(m_tournament->getId()));

    m_networkAdapter.reset(new NetworkAdapter(SELECT_MAILS_MEMBER_TOURNAMENT,SELECT,args,this));
	connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this ,SLOT(extractMails(QStringList, QList<QStringList>)));
}

void TournamentDetail::sendRankingHero(QString)
{
    SendRankingHero sendRankingHero(m_tournament, false, this);
    if (sendRankingHero.exec() == QDialog::Accepted )
    {
        m_tournament->setIsSentToRankingHero(true);
        QStringList args = QStringList();
	    args.append(QString::number(m_tournament->getId()));
	    m_networkAdapter.reset(new NetworkAdapter(UPDATE_TOURNAMENT_SENT_RANKING_HERO,UPDATE,args,this));
	    connect(m_networkAdapter.get(), SIGNAL(finished()), this ,SLOT(close()));
    }
}

void TournamentDetail::importSubscription(QString)
{
    if ( !m_tournament->getIsValidated())
	{
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                   QDir::currentPath(),
                                                    tr("Csv (*.csv)"));
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        qint64 totalSize = file.size();
        QProgressBar progress(nullptr);
        progress.setRange(0, totalSize);
        progress.show();
        QTextStream fileStream(&file);
        QString line = fileStream.readLine(); //Skip first line (header)
        line = fileStream.readLine();
        while(line.compare("") != 0)
        {
            QStringList listLine = line.split(";");
            QString name = listLine.value(0);
            QString firstName = listLine.value(1);
            QString email = listLine.value(2);

            subscribeByEmail(email, name, firstName);

            qint64 readSize = fileStream.pos();
            progress.setValue(readSize);
            line = fileStream.readLine();
        }

        updateList();
        QMessageBox::information(this, "Infos", IMPORT_MULTIPLE_SUCCES);
    }
    else
	{
		QMessageBox::warning(this,INSCRIRE,QString(INSCRIRE_PHRASE));
	}
}

void TournamentDetail::subscribeByEmail(QString email, QString name, QString firstname)
{
    Parameter *param = Parameter::getInstance();
    QUrl url = QUrl(param->getUrl());

    QStringList arg = QStringList();
    arg.append(QString::number(m_tournament->getId()));
    arg.append(email);
    arg.append(name);
    arg.append(firstname);

    QString args = QString("requete=") + QString::number(CREATE_TOURNAMENT_SUBSCRIPTION_MULTIPLE) + QString("&type=") + INSERT + QString("&") + QString("&franchise=") +  QString::number(param->getFranchise()) + QString("&");
	args += NetworkAdapter::formatArguments(arg);

    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.post(QNetworkRequest(url),QByteArray(args.toStdString().c_str()));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    reply->deleteLater();
    QString res = reply->readAll();

    if (!res.isEmpty())
    {
        QStringList tmplist = res.split(HEADER_SEP,QString::SkipEmptyParts);
	    QStringList headers = QStringList();
	    QString result = QString();

	    if (tmplist.size() > 0)
	    {
		    headers = tmplist[0].split(SEPARATOR);
	    }

	    if (tmplist.size() > 1)
	    {
		    QStringList lines = tmplist[1].split(LINE_SEP,QString::SkipEmptyParts);
            result = lines.join(",");
	    }

        m_multipleSubscriptionSelection.reset(new MultipleSubscriptionSelection(m_tournament, result, email, name, firstname, this));
		m_multipleSubscriptionSelection->exec();
		m_multipleSubscriptionSelection.reset();
    }
}

void TournamentDetail::showResultControl(QString)
{
    SendRankingHero sendRankingHero(m_tournament, true, this);
    if (sendRankingHero.exec() == QDialog::Accepted )
    {
        ui.finaliserLabel->setVisible(true);
    }
}

void TournamentDetail::finalizeTournament(QString)
{
    QStringList args = QStringList();
    args.append(QString::number(m_tournament->getId()));

    m_networkAdapter.reset(new NetworkAdapter(UPDATE_FINALISE_TOURNAMENT,UPDATE,args,this));
    QEventLoop loop;
    connect(m_networkAdapter.get(), SIGNAL(finished()), &loop ,SLOT(quit()));
    loop.exec();

    ui.finaliserLabel->setVisible(false);
    //updateList();
    sendRankingHero(QString());
}
