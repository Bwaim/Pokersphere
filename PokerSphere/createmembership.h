#ifndef CREATEMEMBERSHIP_H
#define CREATEMEMBERSHIP_H

#include <QDialog>
#include <QTimer>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include "ui_createmembership.h"
#include "networkadapter.h"
#include "MembershipManager.h"
#include "Member.h"
#include "ChooseSponsor.h"
#include "CreateClub.h"

class CreateMembership : public QDialog
{
	Q_OBJECT

public:
	CreateMembership(QWidget *parent = 0);
	~CreateMembership();

private slots:
    void cancelCreation(const QString&);
	void saveCreation(const QString&);
	void updateCamera();
	void savePhoto(const QString&);
	void startCapture(const QString&);
    void chooseSponsor(bool);
    void updateSponsor();
    void showAddClub(bool);

private:
	Ui::CreateMembershipClass ui;

	std::unique_ptr<NetworkAdapter> m_networkAdapter;
    cv::VideoCapture *m_camera;
	QTimer m_timer;
	bool m_isCaptured;
	MembershipManager *m_membershipManager;
    std::unique_ptr<ChooseSponsor> m_chooseSponsor;
    std::shared_ptr<Member> m_sponsor;
    std::unique_ptr<CreateClub> m_createClub;
};

#endif // CREATEMEMBERSHIP_H
