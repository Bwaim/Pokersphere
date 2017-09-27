#include "Transform.h"
#include "requetes.h"
#include "constantes.h"
#include "MembershipManager.h"
#include "SponsorBonusManagement.h"
#include <QStandardItem>
#include <QBrush>
#include <QPainter>

/*Transform::Transform(void)
{
}


Transform::~Transform(void)
{
}*/

QStandardItemModel*  Transform::toModel(QStringList headers, QList<QStringList> result)
{
	QStandardItemModel*  model = new QStandardItemModel();
	model->setHorizontalHeaderLabels(headers);

	for (int i = 0 ; i < result.size() ; i++)
	{
		QStringList list = result[i];
		//QList<QStandardItem*> row = QList<QStandardItem*>();
		for (int j = 0 ; j < list.size() ; j++)
		{
			if (headers[j] == KOSMOS || headers[j] == KOSMOS_DISPONIBLE || headers[j] == KOSMOS_DISPONIBLE_ENCHERE 
				|| headers[j] == KOSMOS_RESULTAT_NET || headers[j] == KOSMOS_RESULTAT_MOIS_PREC || headers[j] == KOSMOS_RESULTAT_MOIS_COURANT )
			{
				//row.append(new QStandardItem(QString("%L1").arg(list[j].toInt())));
				/*QString val;
				if (list[j].toInt() < 0)
					val = QString("").sprintf("%07d",list[j].toInt());
				else
					val = QString("").sprintf("%06d",list[j].toInt());
				val.insert(val.length()-3,' ');
				row.append(new QStandardItem(val));*/
				QStandardItem *item = new QStandardItem();
				item->setData(QVariant(list[j].toInt()),Qt::DisplayRole);
				//row.append(a);
				model->setItem(i,j,item);
			}
			else if (headers[j] == BIRTH_DATE || headers[j] == ANCIENNETE || headers[j] == ECHEANCE)
			{
				QStandardItem *item = new QStandardItem();
				item->setData(QVariant(QDate::fromString(list[j],QString("dd/MM/yyyy"))),Qt::DisplayRole);
				//row.append(a);
				model->setItem(i,j,item);
			}
			else
				//row.append(new QStandardItem(list[j]));
				model->setItem(i,j,new QStandardItem(list[j]));
		}
		//model->appendRow(row);
	}
	return model;
}

User Transform::toUser(QStringList headers, QList<QStringList> result)
{
	User usr;

	for (int i = 0 ; i < result.size() ; i++)
	{
		QStringList list = result[i];
		for (int j = 0 ; j < list.size() ; j++)
		{
			if (headers[j] == ID_REQ)
				usr.setId(list[j].toInt());
			else 
				if (headers[j] == LOGIN)
					usr.setLogin(list[j]);
				else
					if (headers[j] == ID_GROUP)
						usr.getIdGroup()->setId(list[j].toInt());
					else
						if (headers[j] == NAME)
							usr.getIdGroup()->setName(list[j]);
		}
	}

	return usr;
}

QHash<int, std::shared_ptr<Member>> Transform::toMembers(QStringList headers, QList<QStringList> result)
{
	QHash<int, std::shared_ptr<Member>> listMember;

	for (int i = 0 ; i < result.size() ; i++)
	{
		QStringList list = result[i];
		std::shared_ptr<Member> m = std::make_shared<Member>();
		for (int j = 0 ; j < list.size() ; j++)
		{
			if (headers[j] == ID_REQ)
				m->setId(list[j].toInt());
			else 
				if (headers[j] == NOM)
					m->setName(list[j]);
				else
					if (headers[j] == FIRSTNAME)
						m->setFirstname(list[j]);
					else
						if (headers[j] == PSEUDO)
							m->setPseudo(list[j]);
						else
							if (headers[j] == BIRTH_DATE)
								m->setBirthDate(QDate::fromString(list[j],QString("dd/MM/yyyy")));
							else
								if (headers[j] == ADRESS)
									m->setAdress(list[j]);
								else
									if (headers[j] == ZIP_CODE)
										m->setZipCode(list[j]);
									else
										if (headers[j] == CITY)
											m->setCity(list[j]);
										else
											if (headers[j] == EMAIL)
												m->setEmail(list[j]);
											else
												if (headers[j] == PHONE)
													m->setPhone(list[j]);
												else
													if (headers[j] == KOSMOS)
														m->setKosmos(list[j].toInt());
													else
														if (headers[j] == CLUB)
															m->setClub(list[j]);
														else
															if (headers[j] == ANCIENNETE)
																m->setAnciennete(QDate::fromString(list[j],QString("dd/MM/yyyy")));
															else
																if (headers[j] == ECHEANCE)
																	m->setEcheance(QDate::fromString(list[j],QString("dd/MM/yyyy")));
																else
																	if (headers[j] == RC)
																		m->setRc(list[j].toInt() == 6 ? true : false);
                                                                    else
                                                                        if (headers[j] == CARD_NUMBER)
                                                                            m->setCardNumber(list[j]);
                                                                        else
                                                                            if (headers[j] == ID_UTILISATEUR)
                                                                                m->setLogin(list[j]);
                                                                            else
                                                                                if (headers[j] == SPONSOR_ID)
                                                                                {
                                                                                    if (m->getSponsor() == nullptr)
                                                                                        m->setSponsor(std::make_shared<Member>());
                                                                                    m->getSponsor()->setId(list[j].toInt());
                                                                                }
                                                                                else
                                                                                    if (headers[j] == SPONSOR_NAME)
                                                                                    {
                                                                                        if (m->getSponsor() == nullptr)
                                                                                            m->setSponsor(std::make_shared<Member>());
                                                                                        m->getSponsor()->setName(list[j]);
                                                                                    }
                                                                                    else
                                                                                        if (headers[j] == SPONSOR_FIRSTNAME)
                                                                                        {
                                                                                            if (m->getSponsor() == nullptr)
                                                                                                m->setSponsor(std::make_shared<Member>());
                                                                                            m->getSponsor()->setFirstname(list[j]);
                                                                                        }
                                                                                        else
                                                                                            if (headers[j] == FRANCHISE)
                                                                                                m->setFranchise(list[j]);
                                                                                            else
                                                                                                if (headers[j] == PLAQUE_IMMATRICULATION)
                                                                                                    m->setPlaqueImmatriculation(list[j]);
                                                                                                else
                                                                                                    if (headers[j] == ID_PMU)
                                                                                                        m->setIdPMU(list[j]);
		}
		listMember.insert(m->getId(), m);
	}

	return listMember;
}

QStandardItemModel*  Transform::toModelMembers(QStringList headers, QList<QStringList> result)
{
	return toModel(headers,result);
}

QHash<int, std::shared_ptr<Adhesion>> Transform::toAdhesions(QStringList headers, QList<QStringList> result)
{
	QHash<int, std::shared_ptr<Adhesion>> listAdhesion;

	for (int i = 0 ; i < result.size() ; i++)
	{
		QStringList list = result[i];
		std::shared_ptr<Adhesion> a = std::make_shared<Adhesion>();
		for (int j = 0 ; j < list.size() ; j++)
		{
			if (headers[j] == ID_REQ)
				a->setId(list[j].toInt());
			else 
				if (headers[j] == VALUE)
					a->setValue(list[j].toInt());
				else
					if (headers[j] == OPERATION_DATE)
						a->setOperationDate(QDateTime::fromString(list[j],QString("dd/MM/yyyy HH:mm")));
					else
						if (headers[j] == ENABLE_DATE)
							a->setBeginDate(QDate::fromString(list[j],QString("dd/MM/yyyy")));
						else
							if (headers[j] == DISABLE_DATE)
								a->setEndDate(QDate::fromString(list[j],QString("dd/MM/yyyy")));
                            else
                                if (headers[j] == PRIX)
                                    a->setPrix(list[j].toDouble());
		}
		listAdhesion.insert(a->getId(), a);
	}

	return listAdhesion;
}

QStandardItemModel*  Transform::toModelAdhesions(QStringList headers, QList<QStringList> result)
{
	QStandardItemModel*  model = new QStandardItemModel();
	model->setHorizontalHeaderLabels(headers);
	MembershipManager *membershipManager = MembershipManager::getInstance();

	for (int i = 0 ; i < result.size() ; i++)
	{
		QStringList list = result[i];
		//QList<QStandardItem*> row = QList<QStandardItem*>();
		for (int j = 0 ; j < list.size() ; j++)
		{
			if (headers[j] == VALUE)
				//row.append(new QStandardItem(ABONNEMENT[list[j].toInt()]));
				model->setItem(i,j,new QStandardItem(membershipManager->getLabel(list[j].toInt())));
			else if (headers[j] == OPERATION_DATE)
			{
				QStandardItem *item = new QStandardItem();
				item->setData(QVariant(QDateTime::fromString(list[j],QString("dd/MM/yyyy HH:mm"))),Qt::DisplayRole);
				model->setItem(i,j,item);
			}
			else if (headers[j] == ENABLE_DATE || headers[j] == DISABLE_DATE)
			{
				//row.append(new QStandardItem(list[j]));
				//model->setItem(i,j,new QStandardItem(list[j]));
				QStandardItem *item = new QStandardItem();
				item->setData(QVariant(QDate::fromString(list[j],QString("dd/MM/yyyy"))),Qt::DisplayRole);
				model->setItem(i,j,item);
			}
            else if (headers[j] == PRIX)
				model->setItem(i,j,new QStandardItem(QString::number(list[j].toDouble(),'f',2)));
			else
				model->setItem(i,j,new QStandardItem(list[j]));
		}
		//model->appendRow(row);
	}
	return model;
}

QHash<int, std::shared_ptr<Member>> Transform::toMembersKosmos(QStringList headers, QList<QStringList> result)
{
	QHash<int, std::shared_ptr<Member>> listMember;

	for (int i = 0 ; i < result.size() ; i++)
	{
		QStringList list = result[i];
		std::shared_ptr<Member> m = std::shared_ptr<Member>(std::make_shared<Member>());
		for (int j = 0 ; j < list.size() ; j++)
		{
			if (headers[j] == ID_REQ)
				m->setId(list[j].toInt());
			else 
				if (headers[j] == NOM)
					m->setName(list[j]);
				else
					if (headers[j] == FIRSTNAME)
						m->setFirstname(list[j]);
					else
						if (headers[j] == PSEUDO)
							m->setPseudo(list[j]);
						else
							if (headers[j] == KOSMOS)
								m->setKosmos(list[j].toInt());
							else
								if (headers[j] == KOSMOS_DISPONIBLE)
									m->setKosmosDisponible(list[j].toInt());
								else
									if (headers[j] == KOSMOS_DISPONIBLE_ENCHERE)
										m->setKosmosDisponibleEnchere(list[j].toInt());
									else
										if (headers[j] == KOSMOS_RESULTAT_NET)
											m->setKosmosResultatNet(list[j].toInt());
										else
											if (headers[j] == KOSMOS_RESULTAT_MOIS_PREC)
												m->setKosmosResultatMoisPrecedent(list[j].toInt());
											else
												if (headers[j] == KOSMOS_RESULTAT_MOIS_COURANT)
													m->setKosmosResultatMoisCourant(list[j].toInt());
												else
													if (headers[j] == ECHEANCE)
														m->setEcheance(QDate::fromString(list[j],QString("dd/MM/yyyy")));
                                                    else
                                                        if (headers[j] == FRANCHISE)
                                                            m->setFranchise(list[j]);
		}
		listMember.insert(m->getId(), m);
	}

	return listMember;
}

QStandardItemModel*  Transform::toModelKosmos(QStringList headers, QList<QStringList> result)
{
	return toModel(headers,result);
}

QHash<int, std::shared_ptr<Kosmos>> Transform::toKosmos(QStringList headers, QList<QStringList> result)
{
	QHash<int, std::shared_ptr<Kosmos>> listKosmos;

	for (int i = 0 ; i < result.size() ; i++)
	{
		QStringList list = result[i];
		std::shared_ptr<Kosmos> k = std::make_shared<Kosmos>();
		for (int j = 0 ; j < list.size() ; j++)
		{
			if (headers[j] == ID_REQ)
				k->setId(list[j].toInt());
			else 
				if (headers[j] == VALEUR)
					k->setValue(list[j].toInt());
				else
					if (headers[j] == OPERATION_DATE)
						k->setOperationDate(QDateTime::fromString(list[j],QString("dd/MM/yyyy HH:mm")));
					else
						if (headers[j] == REASON)
							k->setReason(list[j].toInt());
						else
							if (headers[j] == COMMENT)
								k->setComment(list[j]);
							else
								if (headers[j] == CREDIT)
									k->setCredit(list[j].toInt() == 1 ? true : false );
		}
		listKosmos.insert(k->getId(), k);
	}

	return listKosmos;
}

QStandardItemModel*  Transform::toModelKosmosList(QStringList headers, QList<QStringList> result)
{
	QStandardItemModel*  model = new QStandardItemModel();
	model->setHorizontalHeaderLabels(headers);

	for (int i = 0 ; i < result.size() ; i++)
	{
		QStringList list = result[i];
		//QList<QStandardItem*> row = QList<QStandardItem*>();
		bool credit = list[list.size()-1].toInt() == 1 ? true : false;
		QString icon = credit ? QString(":/img/Resources/flecheup.gif") : QString(":/img/Resources/flechedown.gif");
		for (int j = 0 ; j < list.size() ; j++)
		{
			
			if (headers[j] == VALEUR)
			{
				//row.append(new QStandardItem(QIcon(QPixmap(icon)), QString("%L1").arg(list[j].toInt())));
				//model->setItem(i,j,new QStandardItem(QIcon(QPixmap(icon)), QString("%L1").arg(list[j].toInt())));
				QStandardItem *item = new QStandardItem();
				item->setData(QVariant(list[j].toInt()),Qt::DisplayRole);
				item->setIcon(QIcon(QPixmap(icon)));
				model->setItem(i,j,item);
			}
			else
				if (headers[j] == REASON)
					//row.append(new QStandardItem(QString(TYPE_PARTIE_COMPLET[list[j].toInt()])));
					model->setItem(i,j,new QStandardItem(QString(TYPE_PARTIE_COMPLET[list[j].toInt()])));
				else if (headers[j] == OPERATION_DATE)
				{
					QStandardItem *item = new QStandardItem();
					item->setData(QVariant(QDateTime::fromString(list[j],QString("dd/MM/yyyy HH:mm"))),Qt::DisplayRole);
					model->setItem(i,j,item);
				}
				else
					//row.append(new QStandardItem(list[j]));
					model->setItem(i,j,new QStandardItem(list[j]));
		}
		//model->appendRow(row);
	}
	return model;
}

QHash<int, std::shared_ptr<Tournament>> Transform::toTournaments(QStringList headers, QList<QStringList> result)
{
	QHash<int, std::shared_ptr<Tournament>> listTournament;

	for (int i = 0 ; i < result.size() ; i++)
	{
		QStringList list = result[i];
		std::shared_ptr<Tournament> t = std::make_shared<Tournament>();
		for (int j = 0 ; j < list.size() ; j++)
		{
			if (headers[j] == ID_REQ)
				t->setId(list[j].toInt());
			else 
				if (headers[j] == TOURNAMENT)
					t->setName(list[j]);
				else
					if (headers[j] == BEGIN_DATE_TOURNAMENT)
						t->setBeginDate(QDateTime::fromString(list[j],QString("dd/MM/yyyy HH:mm")));
					else
						if (headers[j] == END_REGISTRATION_TOURNAMENT)
							t->setEndRegistrationDate(QDateTime::fromString(list[j],QString("dd/MM/yyyy HH:mm")));
						else
							if (headers[j] == BUY_IN)
							{
								t->setBuyIn(list[j].toInt());
							}
							else
								if (headers[j] == NB_PLAYERS)
									t->setNbPlayers(list[j].toInt());
								else
									if (headers[j] == VARIANTE)
										t->setVariante(list[j].toInt());
									else
										if (headers[j] == NB_SUBSCRIBES)
											t->setNbSubscribes(list[j].toInt());
									else
										if (headers[j] == VALIDATED)
											t->setIsValidated(list[j].toInt() == 1);
										else
											if (headers[j] == COMMENT_TOURNAMENT)
												t->setComment(list[j]);
                                            else
                                                if (headers[j] == PRIVATE)
                                                    t->setPrivate(list[j] == "Oui" ? true : false);
                                                else
                                                    if (headers[j] == SENT_TO_RANKINGHERO)
                                                        t->setIsSentToRankingHero(list[j].toInt() == 1 ? true : false);
                                                    else
                                                        if (headers[j] == FRANCHISE)
                                                            t->setFranchise(list[j].toInt());
                                                        else
                                                            if (headers[j] == ATTACHED_TOURNAMENT)
                                                                t->setAttachedTournament(list[j].toInt());
		}
		listTournament.insert(t->getId(), t);
	}

	return listTournament;
}

QStandardItemModel*  Transform::toModelTournaments(QStringList headers, QList<QStringList> result)
{
	QStandardItemModel*  model = new QStandardItemModel();
	model->setHorizontalHeaderLabels(headers);

	for (int i = 0 ; i < result.size() ; i++)
	{
		QStringList list = result[i];
		//QList<QStandardItem*> row = QList<QStandardItem*>();
		QBrush brush;
		if (headers.size()-1 < list.size())
            if (headers[headers.size()-4] == SENT_TO_RANKINGHERO)
            {
                if (list[headers.size()-4].toInt() == 1)
                {
					brush.setColor(QColor(94,47,106)); // Violet
					brush.setStyle(Qt::SolidPattern);
				}
                else
			        if (headers[headers.size()-3] == VALIDATED)
				        if (list[headers.size()-3].toInt() == 1)
				        {
					        brush.setColor(Qt::green);
					        brush.setStyle(Qt::SolidPattern);
				        }
            }

		for (int j = 0 ; j < list.size() ; j++)
		{
			if (headers[j] == BUY_IN || headers[j] == NB_PLAYERS || headers[j] == NB_SUBSCRIBES)
			{
				//row.append(new QStandardItem(QString("%L1").arg(list[j].toInt())));
				QStandardItem *item = new QStandardItem();
				item->setData(QVariant(list[j].toInt()),Qt::DisplayRole);
				item->setBackground(brush);
				model->setItem(i,j,item);
			}
			else
				if (headers[j] == BEGIN_DATE_TOURNAMENT || headers[j] == END_REGISTRATION_TOURNAMENT)
				{
					QStandardItem *item = new QStandardItem();
					item->setData(QVariant(QDateTime::fromString(list[j],QString("dd/MM/yyyy HH:mm"))),Qt::DisplayRole);
					item->setBackground(brush);
					model->setItem(i,j,item);
				}
				else
					if (headers[j] == VARIANTE)
					{
						//row.append(new QStandardItem(QString(TYPE_VARIANTE[list[j].toInt()])));
						QStandardItem *item = new QStandardItem(QString(TYPE_VARIANTE[list[j].toInt()]));
						item->setBackground(brush);
						model->setItem(i,j,item);
					}
					else
					{
						//row.append(new QStandardItem(list[j]));
						QStandardItem *item = new QStandardItem(list[j]);
						item->setBackground(brush);
						model->setItem(i,j,item);
					}
		}
		//model->appendRow(row);
	}
	return model;
}

QStandardItemModel*  Transform::toModelSubscribesMembers(QStringList headers, QList<QStringList> result)
{
	QStandardItemModel*  model = new QStandardItemModel();
	model->setHorizontalHeaderLabels(headers);
	
	for (int i = 0 ; i < result.size() ; i++)
	{
		QStringList list = result[i];
		
		QBrush brush;
		if (headers.size()-1 < list.size())
			if (headers[headers.size()-4] == ECHEANCE)
			{
				QDate echeance = QDate::fromString(list[headers.size()-4],QString("dd/MM/yyyy"));
				if (echeance < QDate::currentDate())
				{
					brush.setColor(Qt::red);
					brush.setStyle(Qt::SolidPattern);
				}
				else
					if (echeance < QDate::currentDate().addDays(7))
					{
						brush.setColor(Qt::yellow);
						brush.setStyle(Qt::SolidPattern);
					}
			}

		for (int j = 0 ; j < list.size() ; j++)
		{
			if (headers[j] == KOSMOS)
			{
				QStandardItem *item = new QStandardItem();
				item->setData(QVariant(list[j].toInt()),Qt::DisplayRole);
				item->setBackground(brush);
				model->setItem(i,j,item);
			}
			else if (headers[j] == ECHEANCE)
			{
				QStandardItem *item = new QStandardItem();
				item->setData(QVariant(QDate::fromString(list[j],QString("dd/MM/yyyy"))),Qt::DisplayRole);
				item->setBackground(brush);
				model->setItem(i,j,item);
			}
			else if (headers[j] == IS_LATE)
			{
				QStandardItem *item = new QStandardItem();
				if (list[j].toInt() == 1)
					item->setIcon(QIcon(QPixmap(QString(":/img/Resources/clock.png"))));
				model->setItem(i,j,item);
			}
			else
			{
				QStandardItem *item = new QStandardItem(list[j]);
				item->setBackground(brush);
				model->setItem(i,j,item);
			}
		}
		// Pour enlever l'entête du retard
		model->setHorizontalHeaderItem(list.size()-1,new QStandardItem(QString("")));
		// Pour enlever l'entête présent
		model->setHorizontalHeaderItem(list.size()-2,new QStandardItem(QString("")));
		// Pour la création des boutons de désinscription et de sortie du tournoi
		model->setHorizontalHeaderItem(list.size(),new QStandardItem(QString("")));
		model->setItem(i,list.size(),new QStandardItem(QString("")));
	}
	return model;
}

QStandardItemModel*  Transform::toModelAddSubscribeMember(QStringList headers, QList<QStringList> result)
{
	QStandardItemModel*  model = new QStandardItemModel();
	model->setHorizontalHeaderLabels(headers);

	for (int i = 0 ; i < result.size() ; i++)
	{
		QStringList list = result[i];
		
		QBrush brush;
		if (headers.size()-1 < list.size())
			if (headers[headers.size()-1] == ECHEANCE)
			{
				QDate echeance = QDate::fromString(list[headers.size()-1],QString("dd/MM/yyyy"));
				if (echeance < QDate::currentDate())
				{
					brush.setColor(Qt::red);
					brush.setStyle(Qt::SolidPattern);
				}
				else
					if (echeance < QDate::currentDate().addDays(7))
					{
						brush.setColor(Qt::yellow);
						brush.setStyle(Qt::SolidPattern);
					}
			}

		for (int j = 0 ; j < list.size() ; j++)
		{
			if (headers[j] == KOSMOS)
			{
				QStandardItem *item = new QStandardItem();
				item->setData(QVariant(list[j].toInt()),Qt::DisplayRole);
				item->setBackground(brush);
				model->setItem(i,j,item);
			}
			else if (headers[j] == ECHEANCE)
			{
				QStandardItem *item = new QStandardItem();
				item->setData(QVariant(QDate::fromString(list[j],QString("dd/MM/yyyy"))),Qt::DisplayRole);
				item->setBackground(brush);
				model->setItem(i,j,item);
			}
			else
			{
				QStandardItem *item = new QStandardItem(list[j]);
				item->setBackground(brush);
				model->setItem(i,j,item);
			}
		}
	}
	return model;
}

QImage Transform::iplImageToQImage(IplImage *cvimage)
{
	QImage image(cvimage->width, cvimage->height, QImage::Format_RGB32);

	int cvIndex = 0; 
	int cvLineStart = 0;

	for (int y = 0; y < cvimage->height; y++) 
	{
		unsigned char red,green,blue;
		cvIndex = cvLineStart;
		for (int x = 0; x < cvimage->width; x++) 
		{
			red = cvimage->imageData[cvIndex+2];
			green = cvimage->imageData[cvIndex+1];
			blue = cvimage->imageData[cvIndex+0];

			image.setPixel(x,y,qRgb(red, green, blue));
			cvIndex += 3;
		}
		cvLineStart += cvimage->widthStep;                        
	}

	image = image.convertToFormat(QImage::Format_Indexed8,Qt::MonoOnly);

	QVector<QRgb> table;  
	for (int i = 0; i < 256; i++)
		table.append(qRgb(i, i, i));
	image.setColorTable(table);

	return image;
}

QImage  Transform::cvMatToQImage( const cv::Mat &inMat )
{
    switch ( inMat.type() )
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );

        return image;
    }

        // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );

        return image.rgbSwapped();
    }

        // 8-bit, 1 channel
    case CV_8UC1:
    {
        static QVector<QRgb>  sColorTable;

        // only create our color table once
        if ( sColorTable.isEmpty() )
        {
            for ( int i = 0; i < 256; ++i )
                sColorTable.push_back( qRgb( i, i, i ) );
        }

        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );

        image.setColorTable( sColorTable );

        return image;
    }

    default:
        qWarning() << "Transform::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
        break;
    }

    return QImage();
}

QImage Transform::coloredQImageToMonoQImage( const QImage &image)
{
    QImage img = image.convertToFormat(QImage::Format_Indexed8,Qt::MonoOnly);

    QVector<QRgb> table;
    for (int i = 0; i < 256; i++)
        table.append(qRgb(i, i, i));
    img.setColorTable(table);

    return img;
}

QMap<QString, std::shared_ptr<TournamentTemplate>> Transform::toTournamentsTemplate(QStringList headers, QList<QStringList> result)
{
    QMap<QString, std::shared_ptr<TournamentTemplate>> listTournamentTemplate;

	for (int i = 0 ; i < result.size() ; i++)
	{
		QStringList list = result[i];
		std::shared_ptr<TournamentTemplate> t = std::make_shared<TournamentTemplate>();
		for (int j = 0 ; j < list.size() ; j++)
		{
            if (headers[j] == ID_REQ)
                t->setId(list[j].toInt());
            else 
                if (headers[j] == TOURNAMENT)
                    t->setName(list[j]);
                else
                    if (headers[j] == BUY_IN)
                    {
                        t->setBuyIn(list[j].toInt());
                    }
                    else
                        if (headers[j] == NB_PLAYERS)
                            t->setNbPlayers(list[j].toInt());
                        else
                            if (headers[j] == VARIANTE)
                                t->setVariante(list[j].toInt());
                            else
                                if (headers[j] == COMMENT_TOURNAMENT)
                                    t->setComment(list[j]);
										
		}
		listTournamentTemplate.insert(t->getName(), t);
	}

	return listTournamentTemplate;
}

QStandardItemModel* Transform::toModelTournamentsTemplate(TournamentTemplateManager *tournamentsTemplateManager)
{
    QStandardItemModel *model = new QStandardItemModel();
    QStringList headers = tournamentsTemplateManager->getHeaders();
	model->setHorizontalHeaderLabels(headers);

    QMap<QString, std::shared_ptr<TournamentTemplate>> templates = tournamentsTemplateManager->getTemplates();

    QMap<QString, std::shared_ptr<TournamentTemplate>>::iterator i = templates.begin();

    int row = 0;
    while (i != templates.end())
	{
        std::shared_ptr<TournamentTemplate> tournamentTemplate = i.value();

		for (int j = 0 ; j < headers.size() ; j++)
		{
			if (headers[j] == ID_REQ || headers[j] == BUY_IN || headers[j] == NB_PLAYERS)
			{
                int value;
                if (headers[j] == ID_REQ)
                    value = tournamentTemplate->getId();
                else
                {
                    if (headers[j] == BUY_IN)
                        value = tournamentTemplate->getBuyIn();
                    else
                        value = tournamentTemplate->getNbPlayers();
                }

				QStandardItem *item = new QStandardItem();
				item->setData(QVariant(value),Qt::DisplayRole);
				model->setItem(row,j,item);
			}
			else
                if (headers[j] == VARIANTE)
                {
                    QStandardItem *item = new QStandardItem(QString(TYPE_VARIANTE[tournamentTemplate->getVariante()]));
                    model->setItem(row,j,item);
                }
                else
                {
                    if (headers[j] == TOURNAMENT)
                    {
                        QStandardItem *item = new QStandardItem(tournamentTemplate->getName());
                        model->setItem(row,j,item);
                    }
                    else
                    {
                        QStandardItem *item = new QStandardItem(tournamentTemplate->getComment());
                        model->setItem(row,j,item);
                    }
                }
		}
        i++;
        row++;
	}
	return model;
}

QHash<int, std::shared_ptr<SponsorBonus>> Transform::toSponsorBonus(QStringList headers, QList<QStringList> result)
{
    QHash<int, std::shared_ptr<SponsorBonus>> listBonus;

	for (int i = 0 ; i < result.size() ; i++)
	{
		QStringList list = result[i];
		std::shared_ptr<SponsorBonus> b = std::make_shared<SponsorBonus>();
		for (int j = 0 ; j < list.size() ; j++)
		{
			if (headers[j] == ID_REQ)
				b->setId(list[j].toInt());
			else 
				if (headers[j] == LAUNCHING)
					b->setLaunching(list[j].toInt());
				else
					if (headers[j] == TYPE_GODSON_GAIN)
						b->setTypeGodsonGain(list[j].toInt());
					else
						if (headers[j] == GODSON_GAIN)
							b->setGodsonGain(list[j].toInt());
						else
							if (headers[j] == TYPE_SPONSOR_GAIN)
								b->setTypeSponsorGain(list[j].toInt());
                            else
                                if (headers[j] == SPONSOR_GAIN)
                                    b->setSponsorGain(list[j].toInt());
                                else
                                    if (headers[j] == NB_MONTH_FIRST_MEMBERSHIP)
                                        b->setNbMonthFirstMembership(list[j].toInt());
		}
		listBonus.insert(b->getId(), b);
	}

	return listBonus;
}

QStandardItemModel* Transform::toModelSponsorBonus(QStringList headers, QList<QStringList> result)
{
    QStandardItemModel*  model = new QStandardItemModel();
	model->setHorizontalHeaderLabels(headers);
	
	for (int i = 0 ; i < result.size() ; i++)
	{
		QStringList list = result[i];
		for (int j = 0 ; j < list.size() ; j++)
		{
			if (headers[j] == LAUNCHING)
				model->setItem(i,j,new QStandardItem(SponsorBonusManagement::LAUNCHING[list[j].toInt()]));
			else if (headers[j] == TYPE_GODSON_GAIN || headers[j] == TYPE_SPONSOR_GAIN)
			{
				model->setItem(i,j,new QStandardItem(SponsorBonusManagement::BONUS_TYPE[list[j].toInt()]));
			}
			else
				model->setItem(i,j,new QStandardItem(list[j]));
		}
	}
	return model;
}

void Transform::addInfosToPhoto(QPixmap *pixmap, QString franchise, float ratioHeight, QString infos)
{
    int height = pixmap->height();
    int width = pixmap->width();
    float rectHeight = ratioHeight * 25;
    QPainter painter(pixmap);
    QFont font("Arial Narrow",ratioHeight * 20);
    font.setBold(true);
    
    painter.setFont( font );
    painter.fillRect(QRect(0, height-rectHeight,width,rectHeight), QColor(94,47,106));
    painter.drawText( QRect(0, height-rectHeight,width,rectHeight), Qt::AlignCenter, franchise + infos);
}

QStringList Transform::toClubs(QStringList headers, QList<QStringList> result)
{
    QStringList listClub;

    for (int i = 0 ; i < result.size() ; i++)
    {
        QStringList list = result[i];
        for (int j = 0 ; j < list.size() ; j++)
        {
            if (headers[j] == CLUB)
                listClub.append(list[j]);
        }
    }

    return listClub;
}

QStandardItemModel*  Transform::toModelClubs(ClubManager *clubManager)
{
    QStandardItemModel *model = new QStandardItemModel();
    QStringList headers = clubManager->getHeaders();
    model->setHorizontalHeaderLabels(headers);

    QStringList clubs = clubManager->getClubs();

    QStringList::iterator i = clubs.begin();

    int row = 0;
    while (i != clubs.end())
    {
        QString club = *i;

        for (int j = 0 ; j < headers.size() ; j++)
        {
            if (headers[j] == CLUB)
            {
                QStandardItem *item = new QStandardItem(club);
                model->setItem(row,j,item);
            }
        }
        i++;
        row++;
    }
    return model;
}

QMap<QString, std::shared_ptr<FidStatut>> Transform::toFidStatuts(QStringList headers, QList<QStringList> result)
{
    QMap<QString, std::shared_ptr<FidStatut>> listFidStatuts;

    for (int i = 0 ; i < result.size() ; i++)
    {
        QStringList list = result[i];
        std::shared_ptr<FidStatut> t = std::make_shared<FidStatut>();
        for (int j = 0 ; j < list.size() ; j++)
        {
            if (headers[j] == ID_REQ)
                t->setId(list[j].toInt());
            else
                if (headers[j] == STATUT)
                    t->setName(list[j]);
                else
                    if (headers[j] == SEUIL)
                    {
                        t->setSeuil(list[j].toInt());
                    }
        }
        listFidStatuts.insert(t->getName(), t);
    }

    return listFidStatuts;
}

QStandardItemModel* Transform::toModelFidStatuts(FidStatutManager *fidStatutManager)
{
    QStandardItemModel *model = new QStandardItemModel();
    QStringList headers = fidStatutManager->getHeaders();
    model->setHorizontalHeaderLabels(headers);

    QMap<QString, std::shared_ptr<FidStatut>> fidStatuts = fidStatutManager->getFidStatuts();

    QMap<QString, std::shared_ptr<FidStatut>>::iterator i = fidStatuts.begin();

    int row = 0;
    while (i != fidStatuts.end())
    {
        std::shared_ptr<FidStatut> fidStatut = i.value();

        for (int j = 0 ; j < headers.size() ; j++)
        {
            if (headers[j] == ID_REQ || headers[j] == SEUIL)
            {
                int value = 0;
                if (headers[j] == ID_REQ)
                    value = fidStatut->getId();
                else
                {
                    if (headers[j] == SEUIL)
                        value = fidStatut->getSeuil();
                }

                QStandardItem *item = new QStandardItem();
                item->setData(QVariant(value),Qt::DisplayRole);
                model->setItem(row,j,item);
            }
            else
                if (headers[j] == STATUT)
                {
                    QStandardItem *item = new QStandardItem(fidStatut->getName());
                    model->setItem(row,j,item);
                }
        }
        i++;
        row++;
    }
    return model;
}

QHash<int, std::shared_ptr<FidOperation>> Transform::toFidOperations(QStringList headers, QList<QStringList> result)
{
    QHash<int, std::shared_ptr<FidOperation>> listFidOperations;

    for (int i = 0 ; i < result.size() ; i++)
    {
        QStringList list = result[i];
        std::shared_ptr<FidOperation> f = std::make_shared<FidOperation>();
        for (int j = 0 ; j < list.size() ; j++)
        {
            if (headers[j] == ID_REQ)
                f->setId(list[j].toInt());
            else
                if (headers[j] == VALEUR)
                    f->setValue(list[j].toInt());
                else
                    if (headers[j] == OPERATION_DATE)
                        f->setOperationDate(QDateTime::fromString(list[j],QString("dd/MM/yyyy HH:mm")));
                    else
                        if (headers[j] == REASON)
                            f->setType(list[j].toInt());
                        else
                            if (headers[j] == COMMENT)
                                f->setComment(list[j]);
        }
        listFidOperations.insert(f->getId(), f);
    }

    return listFidOperations;
}

QStandardItemModel*  Transform::toModelFidOperationsList(QStringList headers, QList<QStringList> result)
{
    QStandardItemModel*  model = new QStandardItemModel();
    model->setHorizontalHeaderLabels(headers);

    for (int i = 0 ; i < result.size() ; i++)
    {
        QStringList list = result[i];
        for (int j = 0 ; j < list.size() ; j++)
        {

            if (headers[j] == VALEUR)
            {
                QStandardItem *item = new QStandardItem();
                item->setData(QVariant(list[j].toInt()),Qt::DisplayRole);
                model->setItem(i,j,item);
            }
            else
                if (headers[j] == REASON)
                    model->setItem(i,j,new QStandardItem(QString(FID_TYPES_OPERATIONS[list[j].toInt()])));
                else if (headers[j] == OPERATION_DATE)
                {
                    QStandardItem *item = new QStandardItem();
                    item->setData(QVariant(QDateTime::fromString(list[j],QString("dd/MM/yyyy HH:mm"))),Qt::DisplayRole);
                    model->setItem(i,j,item);
                }
                else
                    model->setItem(i,j,new QStandardItem(list[j]));
        }
    }
    return model;
}

QHash<int, std::shared_ptr<Member>> Transform::toMembersFidOperations(QStringList headers, QList<QStringList> result)
{
    QHash<int, std::shared_ptr<Member>> listMember;

    for (int i = 0 ; i < result.size() ; i++)
    {
        QStringList list = result[i];
        std::shared_ptr<Member> m = std::shared_ptr<Member>(std::make_shared<Member>());
        for (int j = 0 ; j < list.size() ; j++)
        {
            if (headers[j] == ID_REQ)
                m->setId(list[j].toInt());
            else
                if (headers[j] == NOM)
                    m->setName(list[j]);
                else
                    if (headers[j] == FIRSTNAME)
                        m->setFirstname(list[j]);
                    else
                        if (headers[j] == PSEUDO)
                            m->setPseudo(list[j]);
                        else
                            if (headers[j] == ECHEANCE)
                                m->setEcheance(QDate::fromString(list[j],QString("dd/MM/yyyy")));
                            else
                                if (headers[j] == FRANCHISE)
                                    m->setFranchise(list[j]);
        }
        listMember.insert(m->getId(), m);
    }

    return listMember;
}

QStandardItemModel*  Transform::toModelFidOperations(QStringList headers, QList<QStringList> result)
{
    return toModel(headers,result);
}

void Transform::toMemberFidPoints(QStringList headers, QList<QStringList> result, std::shared_ptr<Member> member)
{
    for (int i = 0 ; i < result.size() ; i++)
    {
        QStringList list = result[i];
        for (int j = 0 ; j < list.size() ; j++)
        {
            if (headers[j] == POINT)
                member->setFidStatutPoints(list[j].toInt());
            else
                if (headers[j] == POINT_FID)
                    member->setFidPoints(list[j].toInt());
                else
                    if (headers[j] == STATUT)
                        member->setFidStatut(list[j]);
        }
    }
}

QStringList Transform::toDotations(QStringList headers, QList<QStringList> result)
{
    QStringList dotations;

    for (int i = 0 ; i < result.size() ; i++)
    {
        QStringList list = result[i];
        for (int j = 0 ; j < list.size() ; j++)
        {
            if (headers[j] == TYPE)
                dotations.append(list[j]);
            else if (headers[j] == DESCRIPTION)
                dotations.append(list[j]);
        }
    }

    return dotations;
}

QStandardItemModel* Transform::toModelDotation(DotationManager *dotationManager, int mode)
{
    QStandardItemModel *model = new QStandardItemModel();
    QStringList list;
    if (mode == DOTATION_TYPE)
    {
        model->setHorizontalHeaderLabels(QStringList(QString(TYPE)));
        list = dotationManager->getTypes();
    }
    else if (mode == DOTATION_DESCRIPTION)
    {
        model->setHorizontalHeaderLabels(QStringList(QString(DESCRIPTION)));
        list = dotationManager->getDescriptions();
    }

    for (int i = 0; i < list.size(); i++)
    {
        QStandardItem *item = new QStandardItem(list.at(i));
        model->setItem(i,0,item);
    }
    return model;
}
