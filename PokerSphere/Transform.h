#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "User.h"
#include "Member.h"
#include "Adhesion.h"
#include "Kosmos.h"
#include "TournamentTemplate.h"
#include "TournamentTemplateManager.h"
#include "Tournament.h"
#include "SponsorBonus.h"
#include "ClubManager.h"
#include "FidStatut.h"
#include "FidStatutManager.h"
#include "FidOperation.h"
#include "DotationManager.h"
#include <QString>
#include <QList>
#include <QStringList>
#include <QHash>
#include <QStandardItemModel>
#include <QImage>
#include <opencv/cv.h>

class Transform
{
public:
	/*Transform(void);
	~Transform(void);*/

	static User toUser(QStringList headers, QList<QStringList> result);
	static QHash<int, std::shared_ptr<Member>> toMembers(QStringList headers, QList<QStringList> result);
	static QStandardItemModel* toModelMembers(QStringList headers, QList<QStringList> result);
	static QHash<int, std::shared_ptr<Adhesion>> toAdhesions(QStringList headers, QList<QStringList> result);
	static QStandardItemModel* toModelAdhesions(QStringList headers, QList<QStringList> result);
	static QHash<int, std::shared_ptr<Member>> toMembersKosmos(QStringList headers, QList<QStringList> result);
	static QStandardItemModel* toModelKosmos(QStringList headers, QList<QStringList> result);
	static QHash<int, std::shared_ptr<Kosmos>> toKosmos(QStringList headers, QList<QStringList> result);
	static QStandardItemModel* toModelKosmosList(QStringList headers, QList<QStringList> result);
	static QHash<int, std::shared_ptr<Tournament>> toTournaments(QStringList headers, QList<QStringList> result);
	static QStandardItemModel* toModelTournaments(QStringList headers, QList<QStringList> result);
	static QStandardItemModel* toModelSubscribesMembers(QStringList headers, QList<QStringList> result);
	static QStandardItemModel* toModelAddSubscribeMember(QStringList headers, QList<QStringList> result);
	static QImage iplImageToQImage(IplImage *cvImage);
    static QImage cvMatToQImage( const cv::Mat &inMat);
    static QImage coloredQImageToMonoQImage( const QImage &image);
    static QMap<QString, std::shared_ptr<TournamentTemplate>> toTournamentsTemplate(QStringList headers, QList<QStringList> result);
    static QStandardItemModel* toModelTournamentsTemplate(TournamentTemplateManager *tournamentsTemplateManager);
    static QHash<int, std::shared_ptr<SponsorBonus>> toSponsorBonus(QStringList headers, QList<QStringList> result);
	static QStandardItemModel* toModelSponsorBonus(QStringList headers, QList<QStringList> result);
    static void addInfosToPhoto(QPixmap *pixmap, QString franchise, float ratioHeight, QString infos);
    static QStringList toClubs(QStringList headers, QList<QStringList> result);
    static QStandardItemModel* toModelClubs(ClubManager *clubManager);
    static QMap<QString, std::shared_ptr<FidStatut>> toFidStatuts(QStringList headers, QList<QStringList> result);
    static QStandardItemModel* toModelFidStatuts(FidStatutManager *fidStatutManager);
    static QHash<int, std::shared_ptr<FidOperation>> toFidOperations(QStringList headers, QList<QStringList> result);
    static QStandardItemModel* toModelFidOperationsList(QStringList headers, QList<QStringList> result);
    static QHash<int, std::shared_ptr<Member>> toMembersFidOperations(QStringList headers, QList<QStringList> result);
    static QStandardItemModel* toModelFidOperations(QStringList headers, QList<QStringList> result);
    static void toMemberFidPoints(QStringList headers, QList<QStringList> result, std::shared_ptr<Member> member);
    static QStringList toDotations(QStringList headers, QList<QStringList> result);
    static QStandardItemModel* toModelDotation(DotationManager *dotationManager, int mode);

private:
	static QStandardItemModel* toModel(QStringList headers, QList<QStringList> result);

};

#endif // TRANSFORM_H
